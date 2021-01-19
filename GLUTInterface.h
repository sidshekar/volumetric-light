#pragma once

class Game;
class Controls;

class GLUTInterface
{
public:

	static Game* game;
	static Controls* controls;

	static GLUTInterface* getInstance();
	static GLUTInterface *instance;


	GLUTInterface();
	//GLUTInterface(const GLUTInterface* _c);

	static void keyboard(unsigned char key, int x, int y);
	static void keyboard_up(unsigned char key, int x, int y);
	static void mouse(int button, int button_state, int x, int y);
	static void mousePassiveMove(int x, int y);


	static void update(float dt);
	static void render();
	

};

