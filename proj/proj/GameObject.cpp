#include "GameObject.h"

void GameObject::draw(){
	shader->use();

	pushMatrix(MODEL);	
	translate(MODEL, position);
	shader->loadMatrices();

	for (auto mesh : model->meshes) {
		shader->loadMaterial(mesh->MeshMaterial);
		mesh->draw();
	}

	popMatrix(MODEL);
	shader->unUse();
}