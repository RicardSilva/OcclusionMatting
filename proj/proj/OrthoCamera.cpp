#include "OrthoCamera.h"

void OrthoCamera::computeProjection(int w, int h) {
	
	loadIdentity(PROJECTION);
	float ratio = (float)w / h;
	ortho(left, right, bottom , top , nearp, farp);
	
}