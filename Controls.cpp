#include "Controls.h"
#include "Utils.h"


Controls::Controls(){

	yaw = -90.0f;
	pitch = 0.0f;
	lastX = Utils::WIDTH / 2.0;
	lastY = Utils::HEIGHT / 2.0;
	firstMouse = true;
}

Controls::~Controls(){}


void Controls::keyboard(unsigned char key, int x, int y) {
	keyState[key] = BUTTON_DOWN;
}

void Controls::keyboard_up(unsigned char key, int x, int y) {
	keyState[key] = BUTTON_UP;
}

void Controls::mouse(int button, int button_state, int x, int y) {

#define state ( (button_state == GLUT_DOWN) ? BUTTON_DOWN : BUTTON_UP ) // shortens later code

	//printf("mouse xpos: %d, ypos: %d \n", x, y);
	//printf("button %d \n", button);

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mouseState[MOUSE_LEFT] = state;
		//printf("lmb\n");
		break;
	case GLUT_MIDDLE_BUTTON:
		mouseState[MOUSE_MIDDLE] = state;
		//printf("mmb\n");
		break;
	case GLUT_RIGHT_BUTTON:
		mouseState[MOUSE_RIGHT] = state;
		//printf("rmb\n");
		break;
	}
}

void Controls::mousePassiveMove(int x, int y) {

	if (firstMouse) {
		this->lastX = x;
		this->lastY = y;
		firstMouse = false;
	}

	//printf("mouse location: %d, %d \n", x, y);

	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y; // Reversed since y-coordinates go from bottom to left
	lastX = x;
	lastY = y;

	GLfloat sensitivity = 0.5;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}
