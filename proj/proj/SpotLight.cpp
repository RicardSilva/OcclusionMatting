#include "SpotLight.h"

void SpotLight::draw() {

	shader->subLoadSpotLight(*this);

}