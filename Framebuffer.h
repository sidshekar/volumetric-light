#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	unsigned int fbo;
	unsigned int renderTexture;
	bool isColor;

	void createBuffer(bool color);
	void bindBuffer();
	void unBindBuffer();
};

