#include "Framebuffer.h"
#include "utils.h"


Framebuffer::Framebuffer(){}


Framebuffer::~Framebuffer(){}

void Framebuffer::createBuffer(bool color){


	isColor = color;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// position color buffer
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	if(color)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (color){
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	} else{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}


	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("++++++++ Framebuffer not complete! ++++++++++++++ \n");


	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void Framebuffer::bindBuffer(){

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (isColor)
		glClear(GL_COLOR_BUFFER_BIT);
	else
		glClear(GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::unBindBuffer(){

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
