#include "GameManager.h"



void GameManager::onRefreshTimer(int value) {
	/*
	int time = glutGet(GLUT_ELAPSED_TIME);
	int timeStep = time - oldTime;
	oldTime = time;*/
	update(0);
	
	glutPostRedisplay();
}

void GameManager::init() {

	srand(time(NULL));	// initialize seed of random
	fbo = new FrameBuffer();
	virtualColorTexture = fbo->getColorTexture();
	virtualDepthTexture = fbo->getDepthTexture();
	initShaders();
	initLights();
	initMeshes();
	initCameras();
	initGameObjects();
	if (!initKinect())
		std::cout << "error initializing kinect" << std::endl;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, CWIDTH, CHEIGHT,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)colorData);
	glBindTexture(GL_TEXTURE_2D, 0);
	realColorTexture = texture;

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, DWIDTH, DHEIGHT,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)depthData);
	glBindTexture(GL_TEXTURE_2D, 0);
	realDepthTexture = texture;

}

bool GameManager::initKinect() {
	if (FAILED(GetDefaultKinectSensor(&sensor))) {
		return false;
	}
	if (sensor) {
		sensor->Open();

		IColorFrameSource* framesource = NULL;
		sensor->get_ColorFrameSource(&framesource);
		framesource->OpenReader(&colorReader);
		if (framesource) {
			framesource->Release();
			framesource = NULL;
		}
		IDepthFrameSource* framesource2 = NULL;
		sensor->get_DepthFrameSource(&framesource2);
		framesource2->OpenReader(&depthReader);
		if (framesource2) {
			framesource2->Release();
			framesource2 = NULL;
		}
		return true;
	}
	else {
		return false;
	}
}
void GameManager::initShaders() {
	shader = new LightShader("shaders/pointlight.vert", "shaders/pointlight.frag");
	ShaderManager::instance()->addShader("lightShader", shader);

	alphaShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/alphaMatting.frag");
	alphaShader->use();
	alphaShader->bindTextureUnits();
	alphaShader->unUse();
	ShaderManager::instance()->addShader("alphaShader", alphaShader);
}
void GameManager::initLights() {
	directionalLight = new DirectionalLight(vec4(0, 0, -1, 0), vec3(1, 1, 1), 0.5f);
}
void GameManager::initMeshes() {
	ObjLoader* loader = new ObjLoader();
	ObjModel* model;
		
	std::vector<pair<string, string>> modelsToLoad;
	modelsToLoad.push_back(std::make_pair("cube", "objs/cube.obj"));
	modelsToLoad.push_back(std::make_pair("plane", "objs/plane.obj"));

	for (auto m : modelsToLoad) {
		if (loader->LoadFile(m.second)) {
			model = loader->LoadedModel;
			ModelManager::instance()->addModel(m.first, model);
		}
		else
			std::cerr << m.second << ": invalid .obj file" << std::endl;
	}
	//delete(loader);
}
void GameManager::initCameras() {
	// set the camera position based on its spherical coordinates

	loadIdentity(VIEW);

	Camera* sideCamera = new PerspectiveCamera(70, (float)WIDTH / HEIGHT, 0.1f, 150.0f);
	sideCamera->setEye(vec3(0,0,10));
	sideCamera->setTarget(vec3(0,0,0));
	sideCamera->setUp(vec3(0,1,0));
	
	activeCamera = sideCamera;

}
void GameManager::initGameObjects() {
	Shader* shader;
	ObjModel* model;

	shader = ShaderManager::instance()->getShader("lightShader");
	model = ModelManager::instance()->getModel("cube");
	cube = new GameObject(vec3(0,0,0), shader, model);

	shader = ShaderManager::instance()->getShader("alphaShader");
	model = ModelManager::instance()->getModel("plane");
	plane = new GameObject(vec3(0, 0, 0), shader, model);

}



void GameManager::update(double timeStep) {
	getRgbData(colorData);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CWIDTH, CHEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)colorData);
	getDepthData(depthData);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DWIDTH, DHEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)depthData);
}
void GameManager::display() {	
	FrameCount++;
	fbo->bindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fbo->unbindCurrentFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	loadIdentity(MODEL);	
	activeCamera->computeView();
	activeCamera->computeProjection(WIDTH, HEIGHT);

	directionalLight->draw();
	// Render objects
	fbo->bindFrameBuffer();
	cube->draw();
	fbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, virtualColorTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, virtualDepthTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, realColorTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, realDepthTexture);
	
	
	plane->draw();


	glutSwapBuffers();
	
}

void GameManager::getDepthData(GLubyte* dest) {
	IDepthFrame* frame = NULL;
	if (SUCCEEDED(depthReader->AcquireLatestFrame(&frame))) {
		unsigned int sz;
		unsigned short* buf;
		frame->AccessUnderlyingBuffer(&sz, &buf);

		const unsigned short* curr = (const unsigned short*)buf;
		const unsigned short* dataEnd = curr + (DWIDTH*DHEIGHT);

		while (curr < dataEnd) {
			// Get depth in millimeters
			unsigned short depth = (*curr++);

			// Draw a grayscale image of the depth:
			// B,G,R are all set to depth%256, alpha set to 1.
			for (int i = 0; i < 3; ++i)
				*dest++ = (BYTE)depth % 256;
			*dest++ = 0xff;
		}
	}
	if (frame) frame->Release();
}

void GameManager::getRgbData(GLubyte* dest) {
	IColorFrame* frame = NULL;
	if (SUCCEEDED(colorReader->AcquireLatestFrame(&frame))) {
		frame->CopyConvertedFrameDataToArray(CWIDTH*CHEIGHT * 4, colorData, ColorImageFormat_Bgra);
	}
	if (frame) frame->Release();

}





void GameManager::idle() {
	// do nothing
}
void GameManager::keydown(int key) {
	// key = pressed key
	key = tolower(key);
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		break;
	}

}
void GameManager::reshape(GLsizei w, GLsizei h) {
	// When viewport is resized, objects scale with it
	float ratio = (float)WIDTH / HEIGHT;
	float aspect = (float)w / h;

	// adapt viewport
	if (ratio < aspect)
		glViewport((w - h*ratio) / 2.0f, 0, h*ratio, h);
	else
		glViewport(0, (h - w / ratio) / 2, w, w / ratio);
}

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}
void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		getchar();
		exit(EXIT_FAILURE);
	}
}