#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Utils.h"

class Camera
{
public:
	
	//45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f

	Camera();

	Camera(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearplane, GLfloat farPlane);
	~Camera();

	float zNear, zFar, fov, aspectRatio;


	void moveForward();
	void moveBack();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void setCameraSpeed(GLfloat _cameraSpeed);
	void setCameraFront(glm::vec3 _cameraFront);

	void update();

	GLfloat getCameraSpeed();

	glm::mat4 getViewMatrix();
	glm::mat4 getprojectionMatrix();

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraFront();
	glm::vec3 getCameraUp();

	void setCameraPosition(glm::vec3 position);


private:

	GLfloat cameraSpeed;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

};

