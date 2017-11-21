#pragma once

#include <stdlib.h>
#include <ctype.h>
#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>
#include "time.h"

#include "PerspectiveCamera.h"

#include "ShaderManager.h"
#include "ModelManager.h"
#include "ObjLoader.h"
#include "LightShader.h"
#include "AlphaShader.h"
#include "AVTmathLib.h"

#include "Light.h"
#include "DirectionalLight.h"
#include "GameObject.h"
#include "Mesh.h"
#include "FrameBuffer.h"

#include <Ole2.h>
#include <Windows.h>
#include <Kinect.h>


#define DWIDTH 512
#define DHEIGHT 424
#define CWIDTH 1920
#define CHEIGHT 1080

#define WIDTH 1200
#define HEIGHT 900
#define CAPTION "OcclusionMatting"

class GameManager {
	int oldTime = 0;

	// Frame counting and FPS computation
	unsigned int FrameCount = 0;
	char s[32];

	Shader* shader;
	Shader* alphaShader;
	Camera* activeCamera;
	Light* directionalLight;
	GameObject* cube, *plane;
	FrameBuffer* fbo;

	GLuint virtualColorTexture;
	GLuint virtualDepthTexture;
	GLuint realColorTexture;
	GLuint realDepthTexture;

	GLubyte colorData[CWIDTH*CHEIGHT * 4];		// BGRA array that contains the texture data
	GLubyte depthData[DWIDTH*DHEIGHT];			// Depth values stored in this array
	GLubyte colortodepth[DWIDTH*DHEIGHT * 4];	// BGRA array that contains the mapped color data

	
	// Kinect variables
	IKinectSensor* sensor;				// Kinect sensor
	IColorFrameReader* colorReader;     // Kinect color data source
	IDepthFrameReader* depthReader;		// Kinect depth data source


public:
	GameManager() {}

	~GameManager() {
		delete(shader);
		delete(activeCamera);
		delete(directionalLight);
	}


	void onRefreshTimer(int value);

	void init();
	bool initKinect();
	void initShaders();
	void initLights();
	void initMeshes();
	void initCameras();
	void initGameObjects();


	void idle();
	void keydown(int key);

	void getDepthData(GLubyte* dest);
	void getRgbData(GLubyte* dest);

	

	
	void display();

	void update(double timeStep);
	void reshape(GLsizei w, GLsizei h);

};