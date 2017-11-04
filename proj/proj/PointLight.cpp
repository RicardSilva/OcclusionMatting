#include "PointLight.h"

void PointLight::draw() {

	shader->subLoadPointLight(*this);
}