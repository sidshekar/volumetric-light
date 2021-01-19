#pragma once
#include <string>

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

class TextureLoader
{
public:
	TextureLoader();

	GLuint getTextureID(std::string  texFileName);
	~TextureLoader();
};

