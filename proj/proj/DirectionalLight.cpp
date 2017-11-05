#include "DirectionalLight.h"

void DirectionalLight::draw() {

	shader->use();
	shader->subLoadDirectionalLight(*this);
	shader->unUse();
}