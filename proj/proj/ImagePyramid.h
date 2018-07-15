#pragma once

#include <vector>

#include "Shader.h"
#include "AlphaShader.h"
#include "ShaderManager.h"

#include "GameObject.h"


#include "FrameBuffer.h"
#include "MultipleLevelsFrameBuffer.h"
//INPUT IN TEXTURE 10
//OUTPUT EXPANDEND FOREGROUND IN TEXTURE 11
//OUTPUT EXPANDEND BACKGROUND IN TEXTURE 12

class ImagePyramid {

	int imageSource;	 // 0 -> foreground == real color
						 // 1 -> background == virtual color
	GLuint outputTexture;
	GLuint imagePropagationTexture;
	FrameBuffer* downwardPropagationFbo;
	std::vector<FrameBuffer*> upwardPropagationFbos;
	Shader* imagePropagationShader; //compute mip map levels from max to low resolution	
	Shader* cubicInterpolationShader;      //compute final image from lower resolution

	int levels;
	int iterations;

	GameObject *plane;


public: 
	
	ImagePyramid(int imageSource, int levels, int iterations) : imageSource(imageSource), levels(levels), iterations(iterations) {

		downwardPropagationFbo = new MultipleLevelsFrameBuffer(1920, 1080);
		imagePropagationTexture = downwardPropagationFbo->getColorTexture();

		
		glGenTextures(1, &outputTexture);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080,
			0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glGenerateMipmap(GL_TEXTURE_2D);
		/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
		glBindTexture(GL_TEXTURE_2D, 0);

		MultipleLevelsFrameBuffer* mfbo;
		for (int i = 0; i < levels; i++) {
			mfbo = new MultipleLevelsFrameBuffer(1920, 1080, outputTexture, i);
			upwardPropagationFbos.push_back(mfbo);
		
		}

		imagePropagationShader = ShaderManager::instance()->getShader("imagePropagation");
		cubicInterpolationShader = ShaderManager::instance()->getShader("piramidSmoothing");

		ObjModel* model;		
		model = ModelManager::instance()->getModel("plane");
		plane = new GameObject(vec3(0, 0, 0), nullptr, model);

}

	void analize() {	//go from higher level of detail to lower level of detail
		//copy foreground into image E
		//apply propagation shader to image E
		downwardPropagationFbo->bindFrameBuffer();
		imagePropagationShader->use();
		imagePropagationShader->loadImageSource(10);
		imagePropagationShader->loadMode(imageSource);
		plane->draw2();
		imagePropagationShader->unUse();
		downwardPropagationFbo->unbindCurrentFrameBuffer();
			

		//generate mipmap levels for image 
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, imagePropagationTexture);
		glGenerateMipmap(GL_TEXTURE_2D);

	}

	void sintetize() {	//rebuild original level of detail from lower level of detail
		
		for (int level = levels - 1; levels > 0; levels--) {
			//draw low level into next higher level
			upwardPropagationFbos[level-1]->bindFrameBuffer();
			cubicInterpolationShader->use();
			cubicInterpolationShader->loadImageSource(10);
			cubicInterpolationShader->loadMipmapLevel(level);
			cubicInterpolationShader->use();
			upwardPropagationFbos[level-1]->unbindCurrentFrameBuffer();
		
		}

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
	}



};