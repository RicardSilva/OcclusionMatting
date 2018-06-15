#include <math.h>
#include <iostream>
#include <sstream>

#include <string>
#include "GameManager.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#define CAPTION "OcclusionMatting"
int WindowHandle = 0;
int WinX = 1920, WinY = 1080;

GameManager* gm;

// Frame counting and FPS computation
unsigned int FrameCount = 0;
char s[32];

void FPScounter(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, FPScounter, 0);
}

void refreshTimer(int value)
{
	gm->onRefreshTimer(value);
	glutTimerFunc(1000 / 60, refreshTimer, 0);
}

void processKeys(unsigned char key, int xx, int yy)
{
	gm->keydown(key);
}

void reshape(int w, int h) {

	gm->reshape(w, h);
}


void display(void) {
	FrameCount++;
	gm->display();
	
}
void update(void) {
	gm->update(0);

}






int main(int argc, char **argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);


	//  Callback Registration
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);

	glutTimerFunc(0, FPScounter, 0);
	glutTimerFunc(0, refreshTimer, 0);


	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.53, 0.81, 0.92, 0);
	gm = new GameManager();
	gm->init();
	
	//  GLUT main loop
	glutMainLoop();
	delete(gm);
	return(0);

}

