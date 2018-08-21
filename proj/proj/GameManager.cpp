#include "GameManager.h"


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

void GameManager::onRefreshTimer(int value) {	

	update(0);
	glutPostRedisplay();
}

void GameManager::init() {

	srand(time(NULL));	// initialize seed of random

	
	initFrameBuffers();
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, CWIDTH, CHEIGHT,
		0, GL_RED, GL_FLOAT, (float*)depthData);
	glBindTexture(GL_TEXTURE_2D, 0);
	realDepthTexture = texture2;

	foregroundPyramid = new ImagePyramid(0,4,5);
	backgroundPyramid = new ImagePyramid(1,4,5);

}

bool GameManager::initKinect() {
	if (FAILED(GetDefaultKinectSensor(&sensor))) {
		return false;
	}
	if (sensor) {
		sensor->get_CoordinateMapper(&mapper);

		sensor->Open();
		sensor->OpenMultiSourceFrameReader(
			FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color,
			&reader);
		
		return reader;
	}
	else {
		return false;
	}
}
void GameManager::initFrameBuffers() {
	virtualFbo = new FrameBuffer(WIDTH, HEIGHT);
	virtualColorTexture = virtualFbo->getColorTexture();
	virtualDepthTexture = virtualFbo->getDepthTexture();
	frameBuffers.push_back(virtualFbo);

	smoothDepthFbo = new FrameBuffer(WIDTH, HEIGHT);
	smoothRealDepthTexture = smoothDepthFbo->getColorTexture();
	frameBuffers.push_back(smoothDepthFbo);

	invertRealColorFbo = new FrameBuffer(WIDTH, HEIGHT);
	invertedRealColorTexture = invertRealColorFbo->getColorTexture();
	frameBuffers.push_back(invertRealColorFbo);

	coarseTrimapFbo = new FrameBuffer(WIDTH, HEIGHT);
	coarseTrimapTexture = coarseTrimapFbo->getColorTexture();
	frameBuffers.push_back(coarseTrimapFbo);

	trimapEdgeFbo = new FrameBuffer(WIDTH, HEIGHT);
	trimapEdgeTexture = trimapEdgeFbo->getColorTexture();
	frameBuffers.push_back(trimapEdgeFbo);

	realColorEdgeFbo = new FrameBuffer(WIDTH, HEIGHT);
	realColorEdgeTexture = realColorEdgeFbo->getColorTexture();
	frameBuffers.push_back(realColorEdgeFbo);

	unknownLabelsFbo = new FrameBuffer(WIDTH, HEIGHT);
	unknownLabelsTexture = unknownLabelsFbo->getColorTexture();
	frameBuffers.push_back(unknownLabelsFbo);

	finalTrimapFbo = new FrameBuffer(WIDTH, HEIGHT);
	finalTrimapTexture = finalTrimapFbo->getColorTexture();
	frameBuffers.push_back(finalTrimapFbo);



}
void GameManager::initShaders() {
	shader = new LightShader("shaders/pointlight.vert", "shaders/pointlight.frag");
	ShaderManager::instance()->addShader("lightShader", shader);

	depthSmoothingShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/depthSmoothing.frag");
	depthSmoothingShader->use();
	depthSmoothingShader->bindTextureUnits();
	depthSmoothingShader->unUse();
	ShaderManager::instance()->addShader("depthSmoothing", depthSmoothingShader);

	invertRealColorShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/invertRealColor.frag");
	invertRealColorShader->use();
	invertRealColorShader->bindTextureUnits();
	invertRealColorShader->unUse();
	ShaderManager::instance()->addShader("invertRealColor", invertRealColorShader);

	coarseTrimapShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/coarseTrimap.frag");
	coarseTrimapShader->use();
	coarseTrimapShader->bindTextureUnits();
	coarseTrimapShader->unUse();
	ShaderManager::instance()->addShader("coarseTrimap", coarseTrimapShader);

	edgeDetectionShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/edgeDetection.frag");
	edgeDetectionShader->use();
	edgeDetectionShader->bindTextureUnits();
	edgeDetectionShader->unUse();
	ShaderManager::instance()->addShader("edgeDetection", edgeDetectionShader);

	colorEdgeDetectionShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/colorEdgeDetection.frag");
	colorEdgeDetectionShader->use();
	colorEdgeDetectionShader->bindTextureUnits();
	colorEdgeDetectionShader->unUse();
	ShaderManager::instance()->addShader("colorEdgeDetection", colorEdgeDetectionShader);
	
	edgeLabelingShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/edgeLabeling.frag");
	edgeLabelingShader->use();
	edgeLabelingShader->bindTextureUnits();
	edgeLabelingShader->unUse();
	ShaderManager::instance()->addShader("edgeLabeling", edgeLabelingShader);

	unknownDilationShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/unknownDilation.frag");
	unknownDilationShader->use();
	unknownDilationShader->bindTextureUnits();
	unknownDilationShader->unUse();
	ShaderManager::instance()->addShader("unknownDilation", unknownDilationShader);


	Shader* imagePropagationShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/imagePropagation.frag");
	imagePropagationShader->use();
	imagePropagationShader->bindTextureUnits();
	imagePropagationShader->unUse();
	ShaderManager::instance()->addShader("imagePropagation", imagePropagationShader);

	Shader* copyShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/copyShader.frag");
	copyShader->use();
	copyShader->bindTextureUnits();
	copyShader->unUse();
	ShaderManager::instance()->addShader("copy", copyShader);

	Shader* initializeAlphaShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/initializeFAlpha.frag");
	initializeAlphaShader->use();
	initializeAlphaShader->bindTextureUnits();
	initializeAlphaShader->unUse();
	ShaderManager::instance()->addShader("iniAlpha", initializeAlphaShader);

	Shader* preProcessShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/preProcess.frag");
	preProcessShader->use();
	preProcessShader->bindTextureUnits();
	preProcessShader->unUse();
	ShaderManager::instance()->addShader("preProcess", preProcessShader);
	
	Shader* downSamplerShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/downSample.frag");
	downSamplerShader->use();
	downSamplerShader->bindTextureUnits();
	downSamplerShader->unUse();
	ShaderManager::instance()->addShader("pyramidBuilder", downSamplerShader);
	
	Shader* piramidSmoothingShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/cubicInterpolation.frag");
	piramidSmoothingShader->use();
	piramidSmoothingShader->bindTextureUnits();
	piramidSmoothingShader->unUse();
	ShaderManager::instance()->addShader("piramidSmoothing", piramidSmoothingShader);

	Shader* posProcessShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/posProcess.frag");
	posProcessShader->use();
	posProcessShader->bindTextureUnits();
	posProcessShader->unUse();
	ShaderManager::instance()->addShader("posProcess", posProcessShader);

	finalOutputShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/alphaEstimation.frag");
	finalOutputShader->use();
	finalOutputShader->bindTextureUnits();
	finalOutputShader->unUse();
	ShaderManager::instance()->addShader("finalOutput", finalOutputShader);

	

	debugShader = new AlphaShader("shaders/alphaMatting.vert", "shaders/debugShader.frag");
	debugShader->use();
	debugShader->bindTextureUnits();
	debugShader->unUse();
	ShaderManager::instance()->addShader("debug", debugShader);

	
}
void GameManager::initLights() {
	directionalLight = new DirectionalLight(vec4(0, 0, -1, 0), vec3(1, 1, 1), 0.5f);
}
void GameManager::initMeshes() {
	ObjLoader* loader = new ObjLoader();
	ObjModel* model;
		
	std::vector<pair<string, string>> modelsToLoad;
	modelsToLoad.push_back(std::make_pair("cube", "objs/cube2.obj"));
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

	Camera* sideCamera = new PerspectiveCamera(70, (float)WIDTH / HEIGHT, 0.1f, 8000.0f);
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
	cube = new GameObject(vec3(0,0,-730), shader, model);

	shader = ShaderManager::instance()->getShader("alphaShader");
	model = ModelManager::instance()->getModel("plane");
	plane = new GameObject(vec3(0, 0, 0), shader, model);

}



void GameManager::update(double timeStep) {
	getKinectData();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, realColorTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CWIDTH, CHEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)colorData);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, realDepthTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CWIDTH, CHEIGHT, GL_RED, GL_FLOAT, (float*)depthData);
}
void GameManager::display() {	
	FrameCount++;
	glClearColor(1,1,1,0);
	for (auto fbo : frameBuffers) {
		fbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fbo->unbindCurrentFrameBuffer();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	loadIdentity(MODEL);
	
	activeCamera->computeView();
	activeCamera->computeProjection(WIDTH, HEIGHT);

	directionalLight->draw();
	// Render objects
	virtualFbo->bindFrameBuffer();
	cube->draw();
	virtualFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, virtualColorTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, virtualDepthTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, realColorTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, realDepthTexture);
	

	smoothDepthFbo->bindFrameBuffer();
	depthSmoothingShader->use();
	plane->draw2();
	depthSmoothingShader->unUse();
    smoothDepthFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, smoothRealDepthTexture);

	invertRealColorFbo->bindFrameBuffer();
	invertRealColorShader->use();
	plane->draw2();
	invertRealColorShader->unUse();
	invertRealColorFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, invertedRealColorTexture);

	coarseTrimapFbo->bindFrameBuffer();
    coarseTrimapShader->use();
	plane->draw2();
	coarseTrimapShader->unUse();
	coarseTrimapFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, coarseTrimapTexture);

	trimapEdgeFbo->bindFrameBuffer();
	edgeDetectionShader->use();
	plane->draw2();
	edgeDetectionShader->unUse();
	trimapEdgeFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, trimapEdgeTexture);

	realColorEdgeFbo->bindFrameBuffer();
	colorEdgeDetectionShader->use();
	plane->draw2();
	colorEdgeDetectionShader->unUse();
	realColorEdgeFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, realColorEdgeTexture);

	unknownLabelsFbo->bindFrameBuffer();
	edgeLabelingShader->use();
	plane->draw2();
	edgeLabelingShader->unUse();
	unknownLabelsFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, unknownLabelsTexture);

	finalTrimapFbo->bindFrameBuffer();
	unknownDilationShader->use();
	plane->draw2();
	unknownDilationShader->unUse();
	finalTrimapFbo->unbindCurrentFrameBuffer();

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, finalTrimapTexture);

	//TODO: image pyramid stuff
	foregroundPyramid->expandImage();
	backgroundPyramid->expandImage();
	finalOutputShader->use();
	plane->draw2();
	finalOutputShader->unUse();

	/*glViewport(0, 0, 1024, 1024);
	debugShader->use();
	debugShader->loadInputTexture(14);
	plane->draw2();
	debugShader->unUse();*/
	
	

	glutSwapBuffers();
	
}


void GameManager::getDepthData(IMultiSourceFrame* frame, float* dest) {
	IDepthFrame* depthframe;
	IDepthFrameReference* frameref = NULL;
	frame->get_DepthFrameReference(&frameref);
	frameref->AcquireFrame(&depthframe);
	if (frameref) frameref->Release();

	if (!depthframe) return;

	// Get data from frame
	unsigned int sz;
	unsigned short* buf;
	depthframe->AccessUnderlyingBuffer(&sz, &buf);

	// Write vertex coordinates
	mapper->MapColorFrameToDepthSpace(sz, buf, CWIDTH*CHEIGHT, colorDataInDepthSpace);
	float* fdest = (float*)dest;
	int k, j;
	float d;
	for (int i = 0; i < CWIDTH*CHEIGHT; i++) {
		k = colorDataInDepthSpace[i].X;
		j = colorDataInDepthSpace[i].Y;
		if (k > 0 && j > 0 ) {
			d = buf[j * DWIDTH + k];
			if(d == 0)
				fdest[i] = -1.0f;
			else 
				fdest[i] = d / 8000.0f;	
			
			
		}
		else {
				
			fdest[i] = -1.0f;
		}
	}

	if (depthframe) depthframe->Release();

	
}
void GameManager::getColorData(IMultiSourceFrame* frame, GLubyte* dest) {
	IColorFrame* colorframe;
	IColorFrameReference* frameref = NULL;
	frame->get_ColorFrameReference(&frameref);
	frameref->AcquireFrame(&colorframe);
	if (frameref) frameref->Release();

	if (!colorframe) return;

	// Get data from frame
	colorframe->CopyConvertedFrameDataToArray(CWIDTH*CHEIGHT * 4, colorData, ColorImageFormat_Bgra);

	if (colorframe) colorframe->Release();
}
void GameManager::getKinectData() {
	IMultiSourceFrame* frame = NULL;
	if (SUCCEEDED(reader->AcquireLatestFrame(&frame))) {		
		getColorData(frame, colorData);
		getDepthData(frame, depthData);
	}
	if (frame) frame->Release();
}



void GameManager::idle() {
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
	//// When viewport is resized, objects scale with it
	//float ratio = (float)WIDTH / HEIGHT;
	//float aspect = (float)w / h;

	//// adapt viewport
	//if (ratio < aspect)
	//	glViewport((w - h*ratio) / 2.0f, 0, h*ratio, h);
	//else
	//	glViewport(0, (h - w / ratio) / 2, w, w / ratio);
}
