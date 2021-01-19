#include "Game.h"

#include "FMOD\fmod.hpp"

//audio
FMOD::System* audioMgr;

FMOD::Sound* hitSound;
FMOD::Sound* bgMusic;



Game::Game(Controls* _controls) {
	

	controls = _controls;

	FMOD_RESULT result;

	result = FMOD::System_Create(&audioMgr);
	if (result != FMOD_OK)	printf(" audio load error");

	result = audioMgr->init(50, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)	printf(" audio load error");

	//FMOD_DEFAULT - FMOD_LOOP_OFF, FMOD_2D and FMOD_HARDWARE
	//result = audioMgr->createSound("Assets/audio/pop.mp3", FMOD_DEFAULT, 0, &hitSound);
	//result = audioMgr->createSound("Assets/audio/theme.mp3", FMOD_DEFAULT, 0, &bgMusic);

	//FMOD::Channel* channel;
	//audioMgr->playSound(bgMusic, 0, false, &channel);

	 bgMusic->setMode(FMOD_LOOP_NORMAL);



	 ShaderLoader shader;
	 GLuint flatShaderProgram = shader.CreateProgram("Resources/shaders/FlatModel.vs", "Resources/shaders/FlatModel.fs");
	 GLuint gsShaderProgram = shader.CreateProgram("Resources/shaders/FlatModel.vs", "Resources/shaders/FlatModel.fs", "Resources/shaders/gs_ShowNormal.gs");
	 GLuint litTexturedShaderProgram = shader.CreateProgram("Resources/shaders/LitTexturedModel.vs", "Resources/shaders/LitTexturedModel.fs");



	 gBuffer = new GBuffer();
	 gBuffer->createBuffer();

	 lightingFBO = new Framebuffer();
	 lightingFBO->createBuffer(true);

	 shadowMapFBO = new Framebuffer();
	 shadowMapFBO->createBuffer(false);

	 camera = new Camera(45.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 500.0f);
	 camera->setCameraSpeed(0.25f);


	 light = new Light(ModelType::kSphere, camera);
	 light->setProgram(flatShaderProgram);
	 light->setGSProgram(gsShaderProgram);
	 light->setColor(glm::vec3(1.0, 1.0, 1.0));
	 light->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
	 light->setScale(glm::vec3(1.0f));
	 light->setSpeed(0.20f);

	 floor = new GameModel(ModelType::kHQuad, camera, "Resources/images/container.jpg", light, 0.1, 1.5);
	 floor->setProgram(litTexturedShaderProgram);
	 floor->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	 floor->setScale(glm::vec3(40.0f, 40.0f, 40.0f));

	 //vQuad = new GameModel(ModelType::kVQuad, camera, "Resources/images/run.jpg", light, 0.1, 0.0);
	 //vQuad->setProgram(litTexturedShaderProgram);
	 //vQuad->setPosition(glm::vec3(6.0f, 0.0f, 0.0f));
	 //vQuad->setScale(glm::vec3(3.360f / 1.280f, 1.0f, 1.0f));

	 //tri = new GameModel(ModelType::kTriangle, camera, "Resources/images/wall.jpg", light, 0.1, 0.0);
	 //tri->setProgram(litTexturedShaderProgram);
	 //tri->setPosition(glm::vec3(-6.0f, -1.0f, 0.0f));
	 //tri->setScale(glm::vec3(2.0f, 2.0f, 2.0f));
	 
	 cube = new GameModel(ModelType::kCube, camera, "Resources/images/Rayman_origins_man.jpg", light, 0.1, 0.0);
	 cube->setProgram(litTexturedShaderProgram);
	 cube->setPosition(glm::vec3(0.0f, 15.0f, 0.0f));
	 cube->setScale(glm::vec3(1.0f));
	 
	 //bigCube = new GameModel(ModelType::kCube, camera, "Resources/images/earth.jpg", light, 0.1, 0.0);
	 //bigCube->setProgram(litTexturedShaderProgram);
	 //bigCube->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	 //bigCube->setScale(glm::vec3(40.0f, 40.0f, 40.0f));
	
	 //sphere = new GameModel(ModelType::kSphere, camera, "Resources/images/earth.jpg", light, 0.1, 0.0);
	 //sphere->setProgram(litTexturedShaderProgram);
	 //sphere->setPosition(glm::vec3(12.0f, 1.0f, 0.0f));
	 //sphere->setScale(glm::vec3(1.0f, 1.0f, 1.0f));


	 GLuint fullScreenTexLitQuadProgram = shader.CreateProgram("Resources/shaders/ScreenSpaceTexturedLit.vs", "Resources/shaders/ScreenSpaceTexturedLit.fs");
	 GLuint fullScreenTexSSRQuadProgram = shader.CreateProgram("Resources/shaders/ScreenSpaceTexturedSSR.vs", "Resources/shaders/ScreenSpaceTexturedSSR.fs");


	 shadowMapProgram = shader.CreateProgram("Resources/shaders/shadowMap.vs", "Resources/shaders/shadowMap.fs");

	 lighitngFullScreenQuad = new GameModel(ModelType::kVQuad, camera, "Resources/images/container.jpg", light, 0.1, 1.0);
	 lighitngFullScreenQuad->setProgram(fullScreenTexLitQuadProgram);
	 lighitngFullScreenQuad->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	 lighitngFullScreenQuad->setScale(glm::vec3(1.0f, 1.0f, 1.0f));


	 SSRFullScreenQuad = new GameModel(ModelType::kVQuad, camera, "Resources/images/container.jpg", light, 0.1, 1.0);
	 SSRFullScreenQuad->setProgram(fullScreenTexSSRQuadProgram);
	 SSRFullScreenQuad->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	 SSRFullScreenQuad->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

	 //rtQuadSetup();
}

void Game::update(float dt) {


	cube->update(dt);

	camera->update();
	updateKeyboardControls(camera);
}

void Game::render() {

	// +++++++++++++++++
	// ++ shadowmap pass
	// +++++++++++++++++
	shadowMapFBO->bindBuffer();

	cube->shadowMapPass(shadowMapProgram);

	shadowMapFBO->unBindBuffer();

	// ++++++++++++++++
	// ++ gbuffer pass
	// ++++++++++++++++
	gBuffer->bindBuffer();

	//bigCube->renderPass();
	cube->renderPass();
	//sphere->renderPass();
	//tri->renderPass();
	floor->renderPass();
	//vQuad->renderPass();

	light->render();
	
	gBuffer->unBindBuffer();

	// +++++++++++++++++
	// ++ lighting pass
	// +++++++++++++++++

	//lightingFBO->bindBuffer();

	lighitngFullScreenQuad->screenSpaceLightingPass(gBuffer->gPositionTex, gBuffer->gNormalTex, gBuffer->gAlbedoSpecTex, gBuffer->gDepthTex, shadowMapFBO->renderTexture);

	//lightingFBO->unBindBuffer();


	

	//ssQuadPos->screenSpaceRenderPass(gBuffer->gPositionTex, 1.0f);
	//ssQuadNor->screenSpaceRenderPass(gBuffer->gNormalTex, 0.0f);
	//ssQuadCol->screenSpaceRenderPass(gBuffer->gAlbedoSpecTex, -1.0f);
	//ssQuadDep->screenSpaceRenderPass(gBuffer->gDepthTex, -1.0f);
	
	//ssQuadLighting->screenSpaceRenderPass(lightingFBO->lightingRT, -1.0f);

	//SSRFullScreenQuad->screenSpaceReflectionPass(gBuffer->gPositionTex, gBuffer->gNormalTex, gBuffer->gAlbedoSpecTex, lightingFBO-> renderTexture);

}


void Game::updateKeyboardControls(Camera * camera) {

	// camera controls
	if (controls->keyState[(unsigned char)'w'] == BUTTON_DOWN) {
		camera->moveForward();
	}if (controls->keyState[(unsigned char)'s'] == BUTTON_DOWN) {
		camera->moveBack();
	}if (controls->keyState[(unsigned char)'q'] == BUTTON_DOWN) {
		camera->moveUp();
	}if (controls->keyState[(unsigned char)'e'] == BUTTON_DOWN) {
		camera->moveDown();
	}if (controls->keyState[(unsigned char)'a'] == BUTTON_DOWN) {
		camera->moveLeft();
	}if (controls->keyState[(unsigned char)'d'] == BUTTON_DOWN) {
		camera->moveRight();
	}

	//light controls
	if (controls->keyState[(unsigned char)'i'] == BUTTON_DOWN) {
		light->moveForward();
	}if (controls->keyState[(unsigned char)'k'] == BUTTON_DOWN) {
		light->moveBackward();
	}if (controls->keyState[(unsigned char)'j'] == BUTTON_DOWN) {
		light->moveLeft();
	}if (controls->keyState[(unsigned char)'l'] == BUTTON_DOWN) {
		light->moveRight();
	}if (controls->keyState[(unsigned char)'u'] == BUTTON_DOWN) {
		light->moveUp();
	}if (controls->keyState[(unsigned char)'o'] == BUTTON_DOWN) {
		light->moveDown();
	}


	//Cube controls
	if (controls->keyState[(unsigned char)'t'] == BUTTON_DOWN) {
		cube->moveForward();
	}if (controls->keyState[(unsigned char)'g'] == BUTTON_DOWN) {
		cube->moveBackward();
	}if (controls->keyState[(unsigned char)'f'] == BUTTON_DOWN) {
		cube->moveLeft();
	}if (controls->keyState[(unsigned char)'h'] == BUTTON_DOWN) {
		cube->moveRight();
	}if (controls->keyState[(unsigned char)'r'] == BUTTON_DOWN) {
		cube->moveUp();
	}if (controls->keyState[(unsigned char)'y'] == BUTTON_DOWN) {
		cube->moveDown();
	}





	glm::vec3 front;
	front.x = cos(glm::radians(controls->yaw)) * cos(glm::radians(controls->pitch));
	front.y = sin(glm::radians(controls->pitch));
	front.z = sin(glm::radians(controls->yaw)) * cos(glm::radians(controls->pitch));
	camera->setCameraFront(glm::normalize(front));
}


void Game::rtQuadSetup() {


	ShaderLoader shader;

	GLuint ssTexQuadProgram = shader.CreateProgram("Resources/shaders/ScreenSpaceTextured.vs", "Resources/shaders/ScreenSpaceTextured.fs");
	GLuint ssTexDepthQuadProgram = shader.CreateProgram("Resources/shaders/ScreenSpaceTextured.vs", "Resources/shaders/ScreenSpaceTexturedDepth.fs");


	ssQuadPos = new GameModel(ModelType::kVQuad, camera, "Resources/images/container.jpg", light, 0.1, 0.5);
	ssQuadPos->setProgram(ssTexQuadProgram);
	ssQuadPos->setPosition(glm::vec3(0.75f, 0.75f, 0.0f));
	ssQuadPos->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

	ssQuadNor = new GameModel(ModelType::kVQuad, camera, "Resources/images/container.jpg", light, 0.1, 0.5);
	ssQuadNor->setProgram(ssTexQuadProgram);
	ssQuadNor->setPosition(glm::vec3(0.75f, 0.25f, 0.0f));
	ssQuadNor->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

	ssQuadCol = new GameModel(ModelType::kVQuad, camera, "Resources/images/container.jpg", light, 0.1, 0.5);
	ssQuadCol->setProgram(ssTexQuadProgram);
	ssQuadCol->setPosition(glm::vec3(0.75f, -0.75f, 0.0f));
	ssQuadCol->setScale(glm::vec3(0.2f, 0.2f, 0.2f));


	ssQuadLighting = new GameModel(ModelType::kVQuad, camera, "Resources/images/container.jpg", light, 0.1, 0.5);
	ssQuadLighting->setProgram(ssTexQuadProgram);
	ssQuadLighting->setPosition(glm::vec3(-0.75f, 0.75f, 0.0f));
	ssQuadLighting->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

	ssQuadDep = new GameModel(ModelType::kVQuad, camera, "Resources/images/container.jpg", light, 0.1, 0.5);
	ssQuadDep->setProgram(ssTexDepthQuadProgram);
	ssQuadDep->setPosition(glm::vec3(0.75f, -0.25f, 0.0f));
	ssQuadDep->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
	



}
