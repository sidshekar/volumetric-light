#include "SimpleComputeParticle.h"
#include <math.h>
#define NUM_PARTICLES 128 * 1000 
#define SPH_PARTICLE_RADIUS 1.0f


#include "ShaderLoader.h"

#include "shaderhelper.h"

#include "TextureLoader.h"

void SimpleComputeParticle::init(){



	TextureLoader tLoader;
	texture = tLoader.getTextureID("Resources/images/particle4u.png");

	ShaderLoader shaderloader;
	renderProgram = shaderloader.CreateProgram("Resources/shaders/basic.vert", "Resources/shaders/basic.frag", "Resources/shaders/particles.geom");

	computeUpdateProgram = shaderloader.CreateProgram("Resources/shaders/basicUpdate.comp");
	computeSortProgram = shaderloader.CreateProgram("Resources/shaders/basicSort.comp");

	 //ShaderHelper::compileAndLinkShaders("Resources/shaders/basic.vs", "Resources/shaders/basic.fs", renderProgram);

	initialposition.resize(NUM_PARTICLES);
	initialvelocity.resize(NUM_PARTICLES);
	cameraDistance.resize(NUM_PARTICLES);

	float velocity, theta, phi;

		for (int i = 0; i < NUM_PARTICLES; i++) {
		
			initialposition[i] = glm::vec4(0.0f, 
										   0.0f,
										   0.0f, 
										   randomFloat() + 0.125);
			
			initialvelocity[i] = glm::vec4(0.25 * cos(i * .0167 * 0.5) + 0.25f * randomFloat() - 0.125f,
										   1.0f + 0.25f * randomFloat() - 0.125f,
										   0.25 * sin(i* .0167 * 0.5) + 0.25f * randomFloat() - 0.125f,
										   randomFloat() + 0.125);

			cameraDistance[i] = 0.0f;
		}
	

		glGenBuffers(1, &posVbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posVbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, initialposition.size() * sizeof(glm::vec4), &initialposition[0], GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posVbo);

		glGenBuffers(1, &velVbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velVbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, initialvelocity.size() * sizeof(glm::vec4), &initialvelocity[0], GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velVbo);

		glGenBuffers(1, &initVelVbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, initVelVbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, initialvelocity.size() * sizeof(glm::vec4), &initialvelocity[0], GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, initVelVbo);

		glGenBuffers(1, &camDistVbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, camDistVbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, cameraDistance.size() * sizeof(float), &cameraDistance[0], GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, posVbo);
		

		// a useless vao, but we need it bound or we get errors.
		glGenVertexArrays(1, &particleVao);
		glBindVertexArray(particleVao);
		
		glBindBuffer(GL_ARRAY_BUFFER, posVbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


}

void SimpleComputeParticle::render(Camera * camera){


	glUseProgram(computeUpdateProgram);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, camDistVbo);
	GLint camLoc = glGetUniformLocation(computeUpdateProgram, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(camera->getCameraPosition()));

	glDispatchCompute(NUM_PARTICLES / 512, 1, 1);
	// Sync, wait for completion
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glUseProgram(computeSortProgram);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, camDistVbo);


	glDispatchCompute(NUM_PARTICLES / 512, 1, 1);
	// Sync, wait for completion
	glMemoryBarrier(GL_ALL_BARRIER_BITS);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);




	glm::mat4 viewMat = camera->getViewMatrix();

	glm::vec3 vQuad1, vQuad2;

	glm::vec3 vView = camera->getCameraFront();
	vView = glm::normalize(vView);

	vQuad1 = glm::cross(vView, camera->getCameraUp());
	vQuad1 = glm::normalize(vQuad1);

	vQuad2 = glm::cross(vView, vQuad1);
	vQuad2 = glm::normalize(vQuad2);


	glUseProgram(renderProgram);

	glUniform3f(glGetUniformLocation(renderProgram, "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z);
	glUniform3f(glGetUniformLocation(renderProgram, "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);

	glm::mat4 vp = camera->getprojectionMatrix() * camera->getViewMatrix();
	GLint vpLoc = glGetUniformLocation(renderProgram, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(renderProgram, "Texture"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	

	// Bind position buffer as GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
	// Tidy up
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
}
