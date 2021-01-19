#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"


#define BUTTON_UP   0
#define BUTTON_DOWN 1

#define MOUSE_LEFT   0
#define MOUSE_MIDDLE 1
#define MOUSE_RIGHT  2


class Controls
{
public:
	Controls();
	~Controls();

	//movement
	unsigned char keyState[255];
	unsigned char mouseState[3];

	GLfloat yaw;
	GLfloat pitch;
	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse;

	void keyboard(unsigned char key, int x, int y);
	void keyboard_up(unsigned char key, int x, int y);
	void mouse(int button, int button_state, int x, int y);
	void mousePassiveMove(int x, int y);

};

