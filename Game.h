#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "Utils.h"
#include "Camera.h"

#include "GameModel.h"
#include "SimpleComputeParticle.h"
#include "AdvancedComputeParticle.h"

#include "Controls.h"
#include "Light.h"

#include "GBuffer.h"
#include "Framebuffer.h"


class Game {
	
	Light* light;

	GameModel* ssQuadPos;
	GameModel* ssQuadNor;
	GameModel* ssQuadCol;
	GameModel* ssQuadDep;
	GameModel* ssQuadLighting;

	GameModel* lighitngFullScreenQuad;
	GameModel* SSRFullScreenQuad;


	GameModel* floor;
	GameModel* cube;
	GameModel* bigCube;
	GameModel* sphere;
	GameModel* vQuad;
	GameModel* tri;


	GLuint shadowMapProgram;


	Framebuffer* lightingFBO;

	Framebuffer* shadowMapFBO;

	GBuffer* gBuffer;
	 Camera* camera;
	 Controls* controls;

	 void rtQuadSetup();

public:

	Game(Controls* _controls);	 

	void updateKeyboardControls(Camera *camera);

	void render();

	void update(float dt);
};