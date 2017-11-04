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
	if (!pause && !gameOver) {
		int time = glutGet(GLUT_ELAPSED_TIME);
		int timeStep = time - oldTime;
		oldTime = time;
		update(timeStep);
	}
	else {
		oldTime = glutGet(GLUT_ELAPSED_TIME);
	}
	glutPostRedisplay();
}


void GameManager::init() {

	srand(time(NULL));	// initialize seed of random
	
	initShaders();
	initLights();
	initMeshes();
	initCameras();
	initGameObjects();


}

void GameManager::initShaders() {
	shader = new LightShader("shaders/pointlight.vert", "shaders/pointlight.frag");
	ShaderManager::instance()->addShader("lightShader", shader);
}
void GameManager::initLights() {
	directionalLight = new DirectionalLight(vec4(1, -1, 1, 0), vec3(1, 1, 1), 0.5f);
}
void GameManager::initMeshes() {
	ObjLoader* loader = new ObjLoader();
	ObjModel* model;
		
	std::vector<pair<string, string>> modelsToLoad;
	modelsToLoad.push_back(std::make_pair("cube", "objs/cube.obj"));

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
	Shader* shader = ShaderManager::instance()->getShader("lightShader");
	ObjModel* model = ModelManager::instance()->getModel("cube");
	cube = new GameObject(vec3(0,0,0), shader, model);

	//pauseTexture = new TextureHolder("textures/pause_texture.tga", 7);
	//gameOverTexture = new TextureHolder("textures/gameOver_texture.tga", 8);
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

void GameManager::keyup(int key) {
	//// key = pressed key
	//key = tolower(key);
	//switch (key) {
	//case 'd':
	//	car->turnRight = false;
	//	break;
	//case 's':
	//	car->goBack = false;
	//	break;
	//case 'w':
	//	car->goForward = false;
	//	break;
	//case 'a':
	//	car->turnLeft = false;
	//	break;
	//}

}

void GameManager::specialKeydown(int key) {
	/*switch (key) {
	case GLUT_KEY_RIGHT:
		car->turnRight = true;
		break;
	case GLUT_KEY_LEFT:
		car->turnLeft = true;
		break;
	case GLUT_KEY_UP:
		car->goForward = true;
		break;
	case GLUT_KEY_DOWN:
		car->goBack = true;
		break;
	}*/
}

void GameManager::specialKeyup(int key) {
	//switch (key) {
	//case GLUT_KEY_RIGHT:
	//	car->turnRight = false;
	//	break;
	//case GLUT_KEY_LEFT:
	//	car->turnLeft = false;
	//	break;
	//case GLUT_KEY_UP:
	//	car->goForward = false;
	//	break;
	//case GLUT_KEY_DOWN:
	//	car->goBack = false;
	//	break;
	//}
}

void GameManager::mouseButtons(int button, int state, int xx, int yy){}

// Track mouse motion while buttons are pressed

void GameManager::mouseMotion(int xx, int yy) {}

void GameManager::mouseWheel(int wheel, int direction, int x, int y) {}


void GameManager::display() {	
	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadIdentity(MODEL);	
	activeCamera->computeView();
	activeCamera->computeProjection(WIDTH, HEIGHT);

	shader->use();
	directionalLight->draw();
	// Render objects
	cube->draw();

	shader->unUse();
	glutSwapBuffers();
	
}


void GameManager::update(double timeStep) {

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
void GameManager::reshapeAVT(GLsizei w, GLsizei h) {
	float ratio;
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	perspective(53.13f, ratio, 0.1f, 1000.0f);
}