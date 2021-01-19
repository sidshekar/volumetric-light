#include "GLUTInterface.h"
#include "Game.h"
#include "Controls.h"

GLUTInterface* GLUTInterface::instance = NULL;
Game* GLUTInterface::game = NULL;
Controls* GLUTInterface::controls = NULL;

GLUTInterface* GLUTInterface::getInstance(){

	if (instance == nullptr) {
		instance = new GLUTInterface();
	}

	return instance;
}

GLUTInterface ::GLUTInterface() {

	controls = new Controls();
	game = new Game(controls);
}

void GLUTInterface::update(float dt) {
	game->update(dt);
}

void GLUTInterface::render() {
	game->render();
}

void GLUTInterface::keyboard(unsigned char key, int x, int y){
	controls->keyboard(key, x, y);
}

void GLUTInterface::keyboard_up(unsigned char key, int x, int y){
	controls->keyboard_up(key, x, y);
}

void GLUTInterface::mouse(int button, int button_state, int x, int y){
	controls->mouse(button, button_state, x, y);
}

void GLUTInterface::mousePassiveMove(int x, int y){
	controls->mousePassiveMove(x, y);
}
