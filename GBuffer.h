#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	unsigned int gBuffer;
	unsigned int gPositionTex, gNormalTex, gAlbedoSpecTex, gDepthTex;

	void createBuffer();
	void bindBuffer();
	void unBindBuffer();
};

