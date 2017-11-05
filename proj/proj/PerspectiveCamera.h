#pragma once
#include "Camera.h"
class PerspectiveCamera : public Camera {

	float fov;

public:
	PerspectiveCamera(float fov, float ratio, float nearp, float farp) 
		: Camera(nearp, farp), fov(fov){}
	virtual ~PerspectiveCamera() {}

	void computeProjection(int w, int h);


};