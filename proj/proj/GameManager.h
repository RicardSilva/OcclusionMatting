#pragma once

#include <stdlib.h>
#include <ctype.h>
#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>
#include "time.h"

#include "OrthoCamera.h"
#include "PerspectiveCamera.h"

#include "ShaderManager.h"
#include "ModelManager.h"
#include "ObjLoader.h"
#include "LightShader.h"
#include "AVTmathLib.h"
#include "TextureHolder.h"

#include "Light.h"
#include "DirectionalLight.h"
#include "GameObject.h"
#include "Mesh.h"


#define WIDTH 1200
#define HEIGHT 900
#define CAPTION "OcclusionMatting"

class GameManager {

	int oldTime = 0;	// miliseconds

	//GAME LOGIC
	bool pause = false;
	bool gameOver = false;

	// Frame counting and FPS computation
	unsigned int FrameCount = 0;
	char s[32];

	Shader* shader;
	Camera* activeCamera;
	Light* directionalLight;
	GameObject* cube;

public:
	GameManager() {}

	~GameManager() {
		delete(shader);
		delete(activeCamera);
		delete(directionalLight);
	}

	bool getPause() { return pause; }

	void onRefreshTimer(int value);

	void init();
	void initShaders();
	void initLights();
	void initMeshes();
	void initCameras();
	void initGameObjects();


	void idle();

	void keydown(int key);
	void keyup(int key);
	void specialKeydown(int key);
	void specialKeyup(int key);

	void mouseButtons(int button, int state, int xx, int yy);
	void mouseMotion(int xx, int yy);
	void mouseWheel(int wheel, int direction, int x, int y);
	
	void display();

	void update(double timeStep);
	void reshape(GLsizei w, GLsizei h);
	void reshapeAVT(GLsizei w, GLsizei h);

};