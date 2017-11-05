#pragma once
#include "vec3.h"
#include "Shader.h"
#include "Mesh.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "AVTmathLib.h"

#define PI 3.14

class GameObject {


protected:
	Shader* shader;		

public:
	vec3 position;
	ObjModel* model;

	GameObject() {}
	GameObject(vec3& position, Shader* shader, ObjModel* model)
		: position(position), shader(shader), model(model) {
	}
	virtual ~GameObject() {
	}
	
	virtual void draw();
	virtual void draw2();
	virtual void update(float timeStep) {};

};