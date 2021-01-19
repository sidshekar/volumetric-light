//http://in2gpu.com/2014/12/19/create-triangle-opengl-part-iii-attributes/
//http://in2gpu.com/opengl-3/
//http://www.tomdalling.com/blog/modern-opengl/05-model-assets-and-instances/
//http://stackoverflow.com/questions/5668388/drawing-an-array-of-objects-in-open-gl-glut
//http://www.badprog.com/c-opengl-creating-an-equilateral-triangle
//http://in2gpu.com/2014/12/02/create-triangle-opengl-part/
//billbaord/particles - http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
//heightmap - http://nehe.gamedev.net/tutorial/beautiful_landscapes_by_means_of_height_mapping/16006/
//height map - http://www.lighthouse3d.com/opengl/terrain/index.php?heightmap
//tesselation = http://www.informit.com/articles/article.aspx?p=2120983
//source - http://github.prideout.net/
//quad tesselation - http://prideout.net/blog/?p=49
//mirroring - https://open.gl/depthstencils
//shader effects - http://www.geeks3d.com/shader-library/
//raom - http://www.gamasutra.com/view/feature/131596/realtime_dynamic_level_of_detail_.php


//#include "Utils.h"

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include<vector>

#include"GLUTInterface.h"

float prevTime = 0.0f;


void setOpenGLSettings() {

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//GL_FILL
	glEnable(GL_DEPTH_TEST); //enable the depth testing
	glDepthFunc(GL_LESS);

	//enable multisampling
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);
	//glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	//enable culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);// default
		
}

void update() {

	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	currentTime = currentTime / 1000.0f;

	float dt = currentTime - prevTime;


	GLUTInterface::getInstance()->update(currentTime);


	prevTime = currentTime;

	glutPostRedisplay();
}

void render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	GLUTInterface::getInstance()->render();
	
	glutSwapBuffers();
}



int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA| GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 40);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("BASIC GAME");

	//init GLEW
	GLint GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult){
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	setOpenGLSettings();

	GLUTInterface::getInstance();

	glutIdleFunc(update);
	glutDisplayFunc(render);

	glutKeyboardFunc(GLUTInterface::getInstance()->keyboard);
	glutKeyboardUpFunc(GLUTInterface::getInstance()->keyboard_up);
	glutMouseFunc(GLUTInterface::getInstance()->mouse);
	glutPassiveMotionFunc(GLUTInterface::getInstance()->mousePassiveMove);

	glutMainLoop();
	return 0;
}
