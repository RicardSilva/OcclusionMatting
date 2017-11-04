#include "DirectionalLight.h"

void DirectionalLight::draw() {

	shader->subLoadDirectionalLight(*this);

}