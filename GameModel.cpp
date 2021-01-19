
#include "GameModel.h"
#include "Camera.h"


GameModel::GameModel(ModelType modelType, Camera* _camera, std::string texFileName, Light * _light, float _ambientStrength, float _specularStrength) {

	camera = _camera;
	light = _light;
	
	ShaderLoader shaderLoader;
	//shadowMapProgram = shaderLoader.CreateProgram("Resources/shaders/shadowMap.vs", "Resources/shaders/shadowMap.fs");

	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;
	speed = 0.05f;
	rotationAxis = glm::bvec3(0.0f, 0.0f, 1.0f);

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);//if enabled, disables writing to depth
	//glDepthFunc(GL_LESS);//passes if the fragment depth value is less than stored depth value


	switch (modelType){

		case kTriangle: Utils::setTriData(vertices, indices); break;
		case kVQuad: Utils::setVertQuadData(vertices, indices); break;
		case kHQuad: Utils::setHorQuadData(vertices, indices); break;
		case kCube: Utils::setCubeData(vertices, indices); break;
		case kSphere: Utils::setSphereData(vertices, indices); break;
		case kPyramid: Utils::setPyramidData(vertices, indices); break;
	}

	for (auto item : vertices) {
	
		//printf("position: %f, %f, %f \n", item.pos.x, item.pos.y, item.pos.z);
	}

	for (auto item : indices) {

		//printf("index: %d, %d, %d \n", item.x, item.y, item.z);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),	&indices[0], GL_STATIC_DRAW);

	this->setTexture(texFileName);

	//Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,	sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

GameModel::~GameModel(){

}

void GameModel::update(GLfloat time) {

	//model = glm::rotate(model,glm::radians(45.0f) * time, glm::vec3(0.0, 1.0, 0.0f));		

	//model = glm::translate(model, position);

	setRotation(glm::vec3(0.0f, 30.0f * time, 0.0f)) ;
}

void GameModel::renderPass() {

	glUseProgram(this->program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);


	glm::mat4 tmatrix = glm::translate(glm::mat4(), position);

	glm::mat4 rxMatrix = glm::rotate(glm::mat4(), glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 ryMatrix = glm::rotate(glm::mat4(), glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rzMatrix = glm::rotate(glm::mat4(), glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));


	glm::mat4 rMatrix = rxMatrix * ryMatrix * rzMatrix;

	glm::mat4 smatrix = glm::scale(glm::mat4(), scale);

	modelMatrix = tmatrix * rMatrix * smatrix;

	glm::mat4 vp = camera->getprojectionMatrix() * camera->getViewMatrix();
	
	GLint projLoc = glGetUniformLocation(program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));
	
	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLuint specularStrengthLoc = glGetUniformLocation(program, "specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);
	




	glBindVertexArray(vao);		
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	
	glUseProgram(0);

}

void GameModel::screenSpaceRenderPass(GLuint m_texture, GLfloat rtType){

	glUseProgram(this->program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);
	glBindTexture(GL_TEXTURE_2D, m_texture);


	glm::mat4 tmatrix = glm::translate(glm::mat4(), position);

	glm::mat4 rxMatrix = glm::rotate(glm::mat4(), glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 ryMatrix = glm::rotate(glm::mat4(), glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rzMatrix = glm::rotate(glm::mat4(), glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));


	glm::mat4 rMatrix = rxMatrix * ryMatrix * rzMatrix;

	glm::mat4 smatrix = glm::scale(glm::mat4(), scale);

	modelMatrix = tmatrix * rMatrix * smatrix;

	glm::mat4 vp = camera->getprojectionMatrix() * camera->getViewMatrix();
	GLint invViewLoc = glGetUniformLocation(program, "invView");
	glUniformMatrix4fv(invViewLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(camera->getViewMatrix())));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLint intLoc = glGetUniformLocation(program, "rtType");
	glUniform1f(intLoc, rtType);


	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void GameModel::screenSpaceLightingPass(GLuint m_PosTexture, GLuint m_NorTexture, GLuint m_AlbedoTexture, GLuint m_DepthTex, GLuint shadowMapTexture){

	glUseProgram(this->program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "gPosition"), 0);
	glBindTexture(GL_TEXTURE_2D, m_PosTexture);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(program, "gNormal"), 1);
	glBindTexture(GL_TEXTURE_2D, m_NorTexture);

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(program, "gAlbedoSpec"), 2);
	glBindTexture(GL_TEXTURE_2D, m_AlbedoTexture);

	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(program, "gDepth"), 3);
	glBindTexture(GL_TEXTURE_2D, m_DepthTex);

	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(program, "shadowMapTexture"), 4);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);


	glm::mat4 tmatrix = glm::translate(glm::mat4(), position);

	//modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	glm::mat4 rxMatrix = glm::rotate(glm::mat4(), glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 ryMatrix = glm::rotate(glm::mat4(), glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rzMatrix = glm::rotate(glm::mat4(), glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	//modelMatrix = glm::translate(glm::mat4(), glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));

	glm::mat4 rMatrix = rxMatrix * ryMatrix * rzMatrix;

	glm::mat4 smatrix = glm::scale(glm::mat4(), scale);

	modelMatrix = tmatrix * rMatrix * smatrix;

	glm::mat4 vp = camera->getprojectionMatrix() * camera->getViewMatrix();
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLint invViewLoc = glGetUniformLocation(program, "invViewMatrix");
	glUniformMatrix4fv(invViewLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(camera->getViewMatrix())));


	GLint viewLoc = glGetUniformLocation(program, "viewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

	glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 500.0f);
	glUniformMatrix4fv(glGetUniformLocation(program, "invProjectionMatrix") , 1, GL_FALSE, glm::value_ptr(glm::inverse(projMatrix)));

	//shadow
	glm::mat4 lightViewMatrix = glm::lookAt(light->getPosition(),glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)) ;

	GLint lightviewLoc = glGetUniformLocation(program, "lightViewMatrix");
	glUniformMatrix4fv(lightviewLoc, 1, GL_FALSE, glm::value_ptr(lightViewMatrix));




	GLint projectionLoc = glGetUniformLocation(program, "projectionMatrix");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));


	// lighting calculations

	
	GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, this->light->getPosition().x, this->light->getPosition().y, this->light->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, this->light->getColor().x, this->light->getColor().y, this->light->getColor().z);


	GLuint ambientStrengthLoc = glGetUniformLocation(program, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);


	glUniform1f(glGetUniformLocation(program, "zFar"), 20);

	glUniform1f(glGetUniformLocation(program, "zNear"), camera->zNear);

	glUniform1f(glGetUniformLocation(program, "fov"), glm::radians(camera->fov));

	glUniform1f(glGetUniformLocation(program, "aspectRatio"), camera->aspectRatio);






	// Draw
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void GameModel::screenSpaceReflectionPass(GLuint m_PosTexture, GLuint m_NorTexture,  GLuint m_AlbedoTexture, GLuint m_LightingTexture){

	glUseProgram(this->program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "gPosition"), 0);
	glBindTexture(GL_TEXTURE_2D, m_PosTexture);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(program, "gNormal"), 1);
	glBindTexture(GL_TEXTURE_2D, m_NorTexture);

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(program, "gAlbedoSpec"), 2);
	glBindTexture(GL_TEXTURE_2D, m_AlbedoTexture);

	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(program, "gLighting"), 3);
	glBindTexture(GL_TEXTURE_2D, m_LightingTexture);


	glm::mat4 tmatrix = glm::translate(glm::mat4(), position);

	//modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	glm::mat4 rxMatrix = glm::rotate(glm::mat4(), glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 ryMatrix = glm::rotate(glm::mat4(), glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rzMatrix = glm::rotate(glm::mat4(), glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	//modelMatrix = glm::translate(glm::mat4(), glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));

	glm::mat4 rMatrix = rxMatrix * ryMatrix * rzMatrix;

	glm::mat4 smatrix = glm::scale(glm::mat4(), scale);

	modelMatrix = tmatrix * rMatrix * smatrix;

	glm::mat4 vp = camera->getprojectionMatrix() * camera->getViewMatrix();
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLint projLoc = glGetUniformLocation(program, "projMatrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));

	GLint invViewLoc = glGetUniformLocation(program, "invViewMatrix");
	glUniformMatrix4fv(invViewLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(camera->getViewMatrix())));


	// Draw
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void GameModel::shadowMapPass(GLuint shadowMapProgram){
	
	glUseProgram(shadowMapProgram);

	//glm::mat4 lightModelMatrix = glm::mat4();

	//glm::mat4 model;

	//model = glm::translate(model, position);

	//model = glm::translate(model, glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	//model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	//model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	//model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	//model = glm::translate(model, glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));

	//model = glm::scale(model, scale);

	glm::mat4 tmatrix = glm::translate(glm::mat4(), position);

	//modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	glm::mat4 rxMatrix = glm::rotate(glm::mat4(), glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 ryMatrix  = glm::rotate(glm::mat4(), glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rzMatrix = glm::rotate(glm::mat4(), glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	//modelMatrix = glm::translate(glm::mat4(), glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));

	glm::mat4 rMatrix = rxMatrix * ryMatrix * rzMatrix;

	glm::mat4 smatrix = glm::scale(glm::mat4(), scale);

	modelMatrix = tmatrix * rMatrix * smatrix;

	glm::mat4 lightViewMatrix = glm::lookAt(light->getPosition(),  glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 0.0f, 1.0f));
	
	//glm::mat4 lightSpaceMatrix = camera->getprojectionMatrix() * camera->getViewMatrix() * model;


	glm::mat4 lightModelMatrix = glm::translate(glm::mat4(), light->getPosition());

	lightSpaceMatrix = lightViewMatrix;// *lightModelMatrix;


	glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 500.0f);

	GLint pLoc = glGetUniformLocation(shadowMapProgram, "projectionMatrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));

	
	GLint vLoc = glGetUniformLocation(shadowMapProgram, "lightViewMatrix");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	GLint modelLoc = glGetUniformLocation(shadowMapProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}





//movement
void GameModel::moveForward() {

	position -= glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void GameModel::moveBackward() {

	position += glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void GameModel::moveLeft() {

	position -= glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}

void GameModel::moveRight() {

	position += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}

void GameModel::moveUp() {

	position += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}

void GameModel::moveDown() {

	position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}

void GameModel::rotate(glm::vec3 axis){	
	
	this->angle.x += axis.x * speed * 20;
	this->angle.y += axis.y * speed * 20;
	this->angle.z += axis.z * speed * 20;
}

void GameModel::setTexture(std::string  texFileName){

	//** load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, 1280, 720, GL_TRUE);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	//anitrosopic filtering	
	if (glewIsExtensionSupported("GL_EXT_texture_filter_anitrosopic")) {
		
		printf(" Anitrosopic Filtering Supported \n");
		
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}
	else {

		printf(" Anitrosopic Filtering NOT Supported !!! \n");
	
	}
	*/
	
	//** loadImage and create texture
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(texFileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

	printf("fileName %s \n", texFileName.c_str());

	//bIsTextureSet = true;
}

// setters and getters

void GameModel::setScale(glm::vec3 _scale) {


	this->scale = _scale;
}

void GameModel::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void GameModel::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void GameModel::setPosition(glm::vec3 _position) {

	this->position = _position;
}

void GameModel::setColor(glm::vec3 _color) {

	this->color = _color;
}

void GameModel::setCamera(Camera * camera) {

}

void GameModel::setSpeed(float _speed) {

	this->speed = _speed;

}

void GameModel::setProgram(GLuint program) {

	this->program = program;
}



void GameModel::setSpecularStrength(float strength){

	specularStrength = strength;
}

glm::vec3 GameModel::getPosition(){

	return position;
}

glm::vec3 GameModel::getColor(){

	return this->color;
}

glm::vec3 GameModel::getScale(){ 
	return this->scale;
	
}

glm::vec3 GameModel::getRotation(){

	return this->angle;
}

glm::vec3 GameModel::getRotationAxis(){

	return rotationAxis;
}

