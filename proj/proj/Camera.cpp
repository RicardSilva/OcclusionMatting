#include "Camera.h"

void Camera::computeView() {
	
	loadIdentity(VIEW);
	lookAt(eye, target, up);
}

