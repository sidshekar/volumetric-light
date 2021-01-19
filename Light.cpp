#include "Light.h"



Light::Light(ModelType modelType, Camera* camera) {


	_ambientStrength = 0.1f;
	_specularStrength = 0.5f;

	this->camera = camera;

	switch (modelType) {

	case kTriangle: Utils::setTriData(vertices, indices); break;
	case kVQuad: Utils::setVertQuadData(vertices, indices); break;
	case kHQuad: Utils::setHorQuadData(vertices, indices); break;
	case kCube: Utils::setCubeData(vertices, indices); break;
	case kSphere: Utils::setSphereData(vertices, indices); break;
	}

	for (auto item : vertices) {

		//printf("position: %f, %f, %f \n", item.pos.x, item.pos.y, item.pos.z);
	}

	for (auto item : indices) {

		//printf("index: %d, %d, %d \n", item.x, item.y, item.z);
	}

	//glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	
	//Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Light::~Light() {

}

void Light::update(GLfloat time) {

	//printf("position: %f, %f, %f \n", position.x, position.y, position.z);
}


void Light::render() {




	model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);

	glUseProgram(this->program);

	GLint objectColorLoc = glGetUniformLocation(program, "objectColor");
	glUniform3f(objectColorLoc, this->color.x, this->color.y, this->color.z);
	
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = camera->getViewMatrix();
	GLint vLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = camera->getprojectionMatrix();
	GLint pLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(proj));
	
	glEnable(GL_CULL_FACE);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glDisable(GL_CULL_FACE);

	// ++ gs
	glUseProgram(this->gsProgram);

	GLint gsObjectColorLoc = glGetUniformLocation(program, "objectColor");
	glUniform3f(gsObjectColorLoc, 1.0f, 1.0f, 0.0f);

	GLint gsModelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(gsModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	GLint gsVLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(gsVLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint gsPLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(gsPLoc, 1, GL_FALSE, glm::value_ptr(proj));


	this->draw();

	

}

void Light::draw() {

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

//setters
void Light::setScale(glm::vec3 _scale) {

	scale = _scale;
}

void Light::setPosition(glm::vec3 _position) {

	position = _position;	
}

void Light::setColor(glm::vec3 _color) {

	this->color = _color;
}

void Light::setSpeed(float _speed){

	this->speed = _speed;

}

void Light::moveForward(){

	position -= glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void Light::moveBackward(){

	position += glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void Light::moveLeft(){

	position -= glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}

void Light::moveRight(){

	position += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}


void Light::moveUp() {

	position += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}

void Light::moveDown() {

	position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}


void Light::setProgram(GLuint program) {

	this->program = program;
}

void Light::setGSProgram(GLuint program){

	this->gsProgram = program;
}



//getters
glm::vec3 Light::getPosition() {

	return position;
}

glm::vec3 Light::getColor() {
	
	return color;
}

