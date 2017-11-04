#pragma once
#include "Camera.h"
class PerspectiveCamera : public Camera {

	float fov;

	// Camera Spherical Coordinates
	//float alpha = 39.0f, beta = 51.0f;
	//float r = 10.0f;

public:
	PerspectiveCamera(float fov, float ratio, float nearp, float farp) 
		: Camera(nearp, farp), fov(fov){}
	virtual ~PerspectiveCamera() {}

	void computeProjection(int w, int h);

	/*void setAlpha(float a) {
		alpha = a;
	}
	void setBeta(float b) {
		beta = b;
	}
	void setR(float rad) {
		r = rad;
	}*/

};