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

	int mode;     // 0 -> foreground == real color
				  // 1 -> background == virtual color
	int inputImage;
	int outputImage;


	std::vector<GLuint> multiLevelTextures;
	std::vector<FrameBuffer*> multiLevelFbos;



	GLuint copyTexture;
	GLuint outputTexture;

	GLuint imagePropagationTexture;
	FrameBuffer* downwardPropagationFbo;
	std::vector<FrameBuffer*> upwardPropagationFbos;
	FrameBuffer* textureCopyFbo;

	Shader* imagePropagationShader; //compute mip map levels from max to low resolution	
	Shader* cubicInterpolationShader;      //compute final image from lower resolution
	Shader* copyShader;
	Shader* debugShader;

	int levels;
	int iterations;

	GameObject *plane;

	GLubyte mipmapImage0[1920][1080][4];
	GLubyte mipmapImage1[960][540][4];
	GLubyte mipmapImage2[480][270][4];
	GLubyte mipmapImage3[240][135][4];
	GLubyte mipmapImage4[120][67][4];
	GLubyte mipmapImage5[60][33][4];


public: 
	
	ImagePyramid(int mode, int levels, int iterations) : mode(mode), levels(levels), iterations(iterations) {
		// 0 -> foreground == real color
		// 1 -> background == virtual color
		if (mode == 0) {
			inputImage = 2;
			outputImage = 14;
		}
		else if (mode == 1) {
			inputImage = 0;
			outputImage = 15;
		
		}

		textureCopyFbo = new FrameBuffer(1920, 1080);
		copyTexture = textureCopyFbo->getColorTexture();


		downwardPropagationFbo = new MultipleLevelsFrameBuffer(1920, 1080);
		imagePropagationTexture = downwardPropagationFbo->getColorTexture();

		/*glGenTextures(1, &outputTexture);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, levels - 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080,
			0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 960, 540,
			0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 480, 270,
			0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 240, 135,
			0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage3);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 120, 67,
			0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage4);

		MultipleLevelsFrameBuffer* mfbo;
		for (int i = 0; i < levels; i++) {
			mfbo = new MultipleLevelsFrameBuffer(1920, 1080, outputTexture, i);
			upwardPropagationFbos.push_back(mfbo);

		}
		
	
		glBindTexture(GL_TEXTURE_2D, 0);*/

		
		

		imagePropagationShader = ShaderManager::instance()->getShader("imagePropagation");
		cubicInterpolationShader = ShaderManager::instance()->getShader("piramidSmoothing");
		copyShader = ShaderManager::instance()->getShader("copy");
		debugShader = ShaderManager::instance()->getShader("debug");

		ObjModel* model;		
		model = ModelManager::instance()->getModel("plane");
		plane = new GameObject(vec3(0, 0, 0), nullptr, model);


	}

	void expandImage() {

		textureCopyFbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		textureCopyFbo->unbindCurrentFrameBuffer();

		downwardPropagationFbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		downwardPropagationFbo->unbindCurrentFrameBuffer();

		/*for (auto f : upwardPropagationFbos) {
			f->bindFrameBuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			f->unbindCurrentFrameBuffer();
		}*/

		//source -> 10 -> 11 ->output



		
		textureCopyFbo->bindFrameBuffer();
		copyShader->use();
		copyShader->loadImageSource(inputImage);
		plane->draw2();
		copyShader->unUse();
		textureCopyFbo->unbindCurrentFrameBuffer();

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, copyTexture);

		

	
	

		for (int step = 0; step < iterations; step++) {

			//copy input image into image E an
			downwardPropagationFbo->bindFrameBuffer();
			imagePropagationShader->use();
			imagePropagationShader->loadImageSource(10);
			imagePropagationShader->loadMode(mode);
			plane->draw2();
			imagePropagationShader->unUse();
			downwardPropagationFbo->unbindCurrentFrameBuffer();

			glActiveTexture(GL_TEXTURE11);
			glBindTexture(GL_TEXTURE_2D, imagePropagationTexture);
		
			
			glGenerateMipmap(GL_TEXTURE_2D);


			for (int level = levels - 1; level > 0; level--) {
				//draw low level into next higher level
				//upwardPropagationFbos[level-1]->bindFrameBuffer();
				cubicInterpolationShader->use();
				cubicInterpolationShader->loadImageSource(11);
				cubicInterpolationShader->loadMipmapLevel(level);
				plane->draw2();
				cubicInterpolationShader->unUse();
				/*upwardPropagationFbos[level-1]->unbindCurrentFrameBuffer();
				glActiveTexture(GL_TEXTURE11);
				glBindTexture(GL_TEXTURE_2D, outputTexture);*/


				//glCopyTexSubImage2D(GL_TEXTURE_2D, level-1, 0, 0, 0, 0, x, y);
				//glTexImage2D(GL_TEXTURE_2D, level - 1, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				//glActiveTexture(GL_TEXTURE11);
				//glBindTexture(GL_TEXTURE_2D, imagePropagationTexture);
			}
			//glActiveTexture(GL_TEXTURE11);
			//glBindTexture(GL_TEXTURE_2D, imagePropagationTexture);


			//TODO: FINAL STEPS

		}


		

		//glActiveTexture(GL_TEXTURE0 + outputImage);
		//glBindTexture(GL_TEXTURE_2D, outputTexture);
	
	}
	


};