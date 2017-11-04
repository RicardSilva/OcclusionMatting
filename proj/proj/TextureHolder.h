#pragma once

#include "Mesh.h"
#include "ModelManager.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "vec3.h"
#include "TGA.h"
#include "AVTmathLib.h"


class TextureHolder {
	GLint textureID;
	bool isActive;
	vec3 position;
	float width;
	float height;
	Shader* shader;
	ObjModel* model;
	GLuint TextureArray[1];

public:
	TextureHolder(char* file, GLint id) : textureID(id), isActive(true), position(vec3(0,0,0)){
		shader = ShaderManager::instance()->getShader("textureShader");
		model = ModelManager::instance()->getModel("plane");

		if (model == NULL)
			this->isActive = false;

		glGenTextures(1, TextureArray);
		TGA_Texture(TextureArray, file, 0);

	}

	void draw() {
		pushMatrix(MODEL);

		loadIdentity(MODEL);
		translate(MODEL, position);

		shader->loadMatrices();

		//load textures
		if(textureID == 7)
			glActiveTexture(GL_TEXTURE7);
		else
			glActiveTexture(GL_TEXTURE8);

		glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

		shader->loadTexture(textureID);

		for (auto mesh : model->meshes) {
			mesh->draw();
		}


		glBindTexture(GL_TEXTURE_2D, 0);
	}

};