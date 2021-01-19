#pragma once


#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include "ShaderLoader.h";
#include "Utils.h"


class Camera;

#include "Camera.h"

class Light
{

public:
	Light(ModelType modelType, Camera* camera);
	~Light();

	void update(GLfloat time);
	void render();

	void setScale(glm::vec3 _scale);
	void setPosition(glm::vec3 _position);
	void setColor(glm::vec3 _color);
	void setSpeed(float _speed);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void setProgram(GLuint program);
	void setGSProgram(GLuint program);
	void draw();

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 scale;

	glm::vec3 getPosition();
	glm::vec3 getColor();

	glm::mat4 model;

	float _ambientStrength;
	float _specularStrength;

private:


	Camera* camera;

	bool bIsTextureSet = false;

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;

	float speed;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	//glm::mat4 model;
	
	GLuint program;
	GLuint gsProgram;

	

};

