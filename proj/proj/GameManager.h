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
#include "MultipleLevelsFrameBuffer.h"
#include "ImagePyramid.h"

#include <Ole2.h>
#include <Windows.h>
#include <Kinect.h>


#define DWIDTH 512
#define DHEIGHT 424
#define CWIDTH 1920
#define CHEIGHT 1080

#define WIDTH	 1920
#define HEIGHT 1080
#define CAPTION "OcclusionMatting"


#define PIRAMID_LEVELS 1
#define DIFFUSION_STEPS 1

class GameManager {
	int oldTime = 0;
	int outputSwitcher = 0;

	// Frame counting and FPS computation
	unsigned int FrameCount = 0;
	char s[32];

	Shader* shader;
	Shader* depthSmoothingShader;
	Shader* invertRealColorShader;
	Shader* coarseTrimapShader;
	Shader* edgeDetectionShader;
	Shader* colorEdgeDetectionShader;
	Shader* edgeLabelingShader;
	Shader* unknownDilationShader;
	Shader* debugShader;
	Shader* finalShader;

	Shader* finalOutputShader;



	Camera* activeCamera;
	Light* directionalLight;
	GameObject* cube, *plane;

	std::vector<FrameBuffer*> frameBuffers;

	FrameBuffer* virtualFbo;
	FrameBuffer* smoothDepthFbo;
	FrameBuffer* invertRealColorFbo;
	FrameBuffer* coarseTrimapFbo;
	FrameBuffer* trimapEdgeFbo;
	FrameBuffer* realColorEdgeFbo;
	FrameBuffer* unknownLabelsFbo;
	FrameBuffer* finalTrimapFbo;
	FrameBuffer* finalFbos[10];
	


	GLuint virtualColorTexture;
	GLuint virtualDepthTexture;
	GLuint realColorTexture;
	GLuint realDepthTexture;

	GLuint smoothRealDepthTexture;
	GLuint invertedRealColorTexture;
	GLuint coarseTrimapTexture;
	GLuint trimapEdgeTexture;
	GLuint realColorEdgeTexture;
	GLuint unknownLabelsTexture;
	GLuint finalTrimapTexture;	
	GLuint finalTextures[10];


	// Intermediate Buffers
	GLubyte colorData[CWIDTH*CHEIGHT * 4];    // Stores RGBA color image
	float depthData[CWIDTH*CHEIGHT];		  // Stores RGB depth image
	DepthSpacePoint colorDataInDepthSpace[CWIDTH*CHEIGHT];             // Maps depth rgb pixels to depth pixels

														 // Kinect Variables
	IKinectSensor* sensor;             // Kinect sensor
	IMultiSourceFrameReader* reader;   // Kinect data source
	ICoordinateMapper* mapper;         // Converts between depth, color, and 3d coordinates

	ImagePyramid* foregroundPyramid;
	ImagePyramid* backgroundPyramid;



public:
	GameManager() {}

	~GameManager() {
		delete(shader);
		delete(depthSmoothingShader);
		delete(activeCamera);
		delete(directionalLight);
	}


	void onRefreshTimer(int value);

	void init();
	bool initKinect();
	void initFrameBuffers();
	void initShaders();
	void initLights();
	void initMeshes();
	void initCameras();
	void initGameObjects();


	void idle();
	void keydown(int key);

	void getKinectData();
	void getDepthData(IMultiSourceFrame* frame, float* dest);
	void getColorData(IMultiSourceFrame* frame, GLubyte* dest);
	
	
	void display();

	void update(double timeStep);
	void reshape(GLsizei w, GLsizei h);

};