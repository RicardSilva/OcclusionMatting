#include "GameObject.h"

void GameObject::draw(){
	shader->use();

	pushMatrix(MODEL);	
	translate(MODEL, position);
	scale(MODEL, 3, 3, 3);
	shader->loadMatrices();

	for (auto mesh : model->meshes) {
		shader->loadMaterial(mesh->MeshMaterial);
		mesh->draw();
	}

	popMatrix(MODEL);
	shader->unUse();
}

void GameObject::draw2() {
	
	for (auto mesh : model->meshes) {
		mesh->draw();
	}

}