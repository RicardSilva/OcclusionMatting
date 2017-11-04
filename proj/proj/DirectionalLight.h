#pragma once

#include "Light.h"
#include "vec3.h"

struct DirectionalLight : public Light {
	
	void draw();

	DirectionalLight(vec4& direction, vec3& color, float intensity) : Light() {
		this->type = directionalLight;
		this->direction = direction;
		this->color = color;
		this->intensity = intensity;
		shader->use();
		shader->loadDirectionalLight(*this);
		shader->unUse();
	}


};