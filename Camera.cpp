#include "Camera.h"

#include <iostream>

Camera::Camera()
{
}

Camera::Camera(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearplane, GLfloat farPlane){

	cameraPos = glm::vec3(0.0f, 1.0f, 20.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -5.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	zNear = nearplane;
	zFar = farPlane;
	fov = FOV;
	aspectRatio = width / height;


	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projectionMatrix = glm::perspective(glm::radians(FOV), width /height, nearplane, farPlane);

	//projectionMatrix = glm::ortho(0.0f,width, 0.0f, height, 1.0f , 100.0f);
}


Camera::~Camera(){

}

void Camera::update(){

	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}


void Camera::moveForward(){

	cameraPos += cameraSpeed * cameraFront;

	//printf("cameraMove: %f \n", cameraPos.x);

}


void Camera::moveBack(){

	cameraPos -= cameraSpeed * cameraFront;
}

void Camera::moveUp(){

	cameraPos += cameraSpeed * cameraUp;
}

void Camera::moveDown(){

	cameraPos -= cameraSpeed * cameraUp;
}

void Camera::moveLeft(){

	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::moveRight(){

	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


// 

glm::mat4 Camera::getViewMatrix(){

	return viewMatrix;

}
glm::mat4 Camera::getprojectionMatrix(){

	return projectionMatrix;
}

glm::vec3 Camera::getCameraPosition(){

	return cameraPos;
}

glm::vec3 Camera::getCameraFront(){

	return cameraFront;
}

glm::vec3 Camera::getCameraUp(){

	return cameraUp;
}

void Camera::setCameraPosition(glm::vec3 position){

	cameraPos = position;
}

GLfloat Camera::getCameraSpeed(){

	return cameraSpeed;
}

void Camera::setCameraSpeed(GLfloat _cameraSpeed){

	cameraSpeed = _cameraSpeed;
}

void Camera::setCameraFront(glm::vec3 _cameraFront){

	cameraFront = _cameraFront;
}