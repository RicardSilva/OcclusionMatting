#include "PerspectiveCamera.h"

void PerspectiveCamera::computeProjection(int w, int h) {

	float ratio = (float)w / h;
	loadIdentity(PROJECTION);
	perspective(fov, ratio, nearp, farp);
}