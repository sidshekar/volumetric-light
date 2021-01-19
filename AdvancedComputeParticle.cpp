#include "AdvancedComputeParticle.h"

#include "ShaderLoader.h"

void AdvancedComputeParticle::init(){

	//int i;

	ShaderLoader shaderloader;

	render_prog = shaderloader.CreateProgram("Resources/shaders/advancedCompute.vs",
											 "Resources/shaders/advancedCompute.fs");
	compute_prog = shaderloader.CreateProgram("Resources/shaders/advancedCompute.cs");

	
	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);

	//position and velocity buffers
	// Generate two buffers, bind them and initialize their data stores
	glGenBuffers(2, buffers);

	//////////////////////////////
	//- Initialise position buffer
	//////////////////////////////
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	// Map the position buffer
	glm::vec4 * positions = (glm::vec4 *)glMapBufferRange(GL_ARRAY_BUFFER,//target
		0,//offset
		PARTICLE_COUNT * sizeof(glm::vec4), //buffer length
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT); //access type

	// Fill it with random vectors

	for (int i = 0; i < PARTICLE_COUNT; i++) {
		positions[i] = glm::vec4(random_vector(-10.0f, 10.0f), // positon near the origin
			random_float()); // life exceptancy
	}

	//unmap buffer
	glUnmapBuffer(GL_ARRAY_BUFFER);

	////////////////////////
	//- set vertex attribute
	////////////////////////
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	////////////////////////////////////////
	// Initialization of the velocity buffer
	////////////////////////////////////////
	glBindBuffer(GL_ARRAY_BUFFER, velocity_buffer);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);

	// Map the velocity buffer
	glm::vec4 * velocities = (glm::vec4 *)glMapBufferRange(GL_ARRAY_BUFFER,0,PARTICLE_COUNT * sizeof(glm::vec4),GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	// Filled with random vectors
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		velocities[i] = glm::vec4(random_vector(-0.1f, 0.1f), 0.0f);
	}
	//unmap buffer
	glUnmapBuffer(GL_ARRAY_BUFFER);

	/////////////////////////
	//Bind to texture buffers
	/////////////////////////
	glGenTextures(2, tbos);
	for (int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_BUFFER, tbos[i]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, buffers[i]);// bind buffers to textures
	}

	///////////////////
	//attractor buffers
	///////////////////
	glGenBuffers(1, &attractor_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, attractor_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);

	// masses for attractors are between 0.5 and 1.0
	// positions are initilized to 0 , but are moved during the render loop
	for (int i = 0; i < MAX_ATTRACTORS; i++) {
		attractor_masses[i] = 0.5f + random_float() * 0.5f;
	}

	// Now bind the buffer to the zeroth GL_SHADER_STORAGE_BUFFER binding point
	glBindBufferBase(GL_UNIFORM_BUFFER,// target buffer type
		0,//index of the binding point
		attractor_buffer);//name of targret buffer
	
	glUseProgram(compute_prog);
	dt_location = glGetUniformLocation(compute_prog, "dt");
	
}

void AdvancedComputeParticle::render(){

	static const GLuint start_ticks = ::GetTickCount() - 100000;
	GLuint current_ticks = ::GetTickCount();
	static GLuint last_ticks = current_ticks;
	float time = ((start_ticks - current_ticks) & 0xFFFFF) / float(0xFFFFF);
	float delta_time = (float)(current_ticks - last_ticks) * 0.075f;

	//printf("delta time :  %f \n", delta_time);

	////////////////////////////////////
	//download attractor buffers content
	////////////////////////////////////

	glm::vec4 * attractors = (glm::vec4 *)glMapBufferRange(GL_UNIFORM_BUFFER, //target 
		0, //offset
		32 * sizeof(glm::vec4), //length
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);//access

	int i;
	//changing the position of the attractors
	for (i = 0; i < 32; i++) {
		attractors[i] = glm::vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,// set x
			cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f, // set y
			sinf(time * (float)(i + 3) * 5.3f * 20.0f) * cosf(time * (float)(i + 5) * 9.1f) * 100.0f, // set z
			attractor_masses[i]); // get weights from attractor masses array
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);


	// If dt is too large, the system could explode, so cap it to
	// some maximum allowed value
	if (delta_time >= 2.0f) {
		delta_time = 2.0f;
	}

	// Activate the compute program and bind the position and velocity buffers
	glUseProgram(compute_prog);
	glBindImageTexture(0, // location id 
		velocity_tbo, // buffer id
		0, // level
		GL_FALSE, // layered
		0, // layer
		GL_READ_WRITE, //access type 
		GL_RGBA32F); // image format

	glBindImageTexture(1, position_tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	// Set delta time
	glUniform1f(dt_location, delta_time);

	// Dispatch
	glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -60.0f));
	model = glm::rotate(model, glm::radians(time * 1000.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mvp = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 1000.0f) *	model;

	// Clear, select the rendering program and draw a full screen quad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);


	glUseProgram(render_prog);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(render_vao);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glPointSize(2.0f);
	glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

	last_ticks = current_ticks;

	//int workGroupSize[3];
	//glGetProgramiv(compute_prog, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
	//printf("Local Work group size is %d x %d x %d items.\n", workGroupSize[0], workGroupSize[1], workGroupSize[2]);

}
