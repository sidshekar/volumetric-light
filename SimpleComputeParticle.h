#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"

#include <vector>


#include "Camera.h"


#include <random>


static float randFloat(float min, float max) {
	float r = (float)rand() / (double)RAND_MAX;
	return min + r * (max - min);
}

static float randomFloat() {
	float r = (float)rand() / (double)RAND_MAX;
	return r;
}


class SimpleComputeParticle
{
public:


	void init();

	void render(Camera* camera);


	std::vector<glm::vec4> initialposition;
	std::vector<glm::vec4> initialvelocity;
	std::vector<float> cameraDistance;

	GLuint posVbo, velVbo, initVelVbo, camDistVbo, particleVao, texture;
	GLuint computeUpdateProgram, computeSortProgram, renderProgram;



	

};

