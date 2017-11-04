#pragma once
#include "Light.h"

struct PointLight : public Light {


	PointLight(vec4& position, vec3& color, float intensity) : Light() {
	
		this->type = pointLight;
		this->position = position;
		this->color = color;
		this->intensity = intensity;
		this->constantAttenuation = 0.5;
		this->linearAttenuation = 0.001;
		this->quadraticAttenuation = 0.00001;
		shader->use();
		shader->loadPointLight(*this);
		shader->unUse();
	}
	virtual ~PointLight() {
	}

	void updatePosition(vec3 pos) {
		position.x = pos.x;
		position.z = pos.z;
	}

	void draw();
};