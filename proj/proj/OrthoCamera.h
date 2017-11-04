#pragma once
#include "Camera.h"
class OrthoCamera : public Camera {
	float left, right;
	float bottom, top;


public:
	
	OrthoCamera(float left, float right,
		float bottom, float top,
		float nearp, float farp) 
		: Camera(nearp, farp), left(left), right(right), bottom(bottom), top(top) {}

	virtual ~OrthoCamera() {}

	void computeProjection(int w, int h);


};