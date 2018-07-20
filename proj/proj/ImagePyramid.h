#pragma once

#include <vector>
#include <iostream>

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


	int levels;
	int iterations;

	int mode;     // 0 -> foreground == real color
				  // 1 -> background == virtual color
	int outputImage;


	std::vector<GLuint> multiLevelTextures;
	std::vector<FrameBuffer*> multiLevelFbos;


	GLuint textureF;
	GLuint iniTextureF;
	GLuint textureS;
	GLuint finalTextureF;

	GLuint preProcessedTextureS;
	GLuint outputTexture;


	FrameBuffer* inputCreationFbo;
	FrameBuffer* initAlphaFbo;
	FrameBuffer* textureCopyFbo;
	FrameBuffer* finalFbo;

	Shader* initializeShader;
	Shader* iniAlphaShader;
	Shader* copyShader;
	Shader* preProcessShader; 
	Shader* pyramidBuilderShader;    //compute image levels from max to low resolution	
	Shader* pyramidSmoothingShader;  //compute final image from lower resolution
	Shader* posProcessShader;

	Shader* debugShader;
	GameObject *plane;


public: 
	
	ImagePyramid(int mode, int levels, int iterations) : mode(mode), levels(levels), iterations(iterations) {
		// 0 -> foreground == real color
		// 1 -> background == virtual color
		if (mode == 0) {
			outputImage = 14;
		}
		else if (mode == 1) {
			outputImage = 15;
		
		}

		inputCreationFbo = new FrameBuffer(1024, 1024);
		textureF = inputCreationFbo->getColorTexture();

		initAlphaFbo = new FrameBuffer(1024, 1024);
		iniTextureF = initAlphaFbo->getColorTexture();

		textureCopyFbo = new FrameBuffer(1024, 1024);
		textureS = textureCopyFbo->getColorTexture();
		finalFbo = new FrameBuffer(1024, 1024);
		finalTextureF = finalFbo->getColorTexture();



		//Image pyramid
		for (int i = 0; i < levels; i++) {
			int div = pow(2, i);
			int width = 1024 / div;
			int height = 1024 / div;

			FrameBuffer* fbo = new FrameBuffer(width, height);
			GLuint texture = fbo->getColorTexture();

			multiLevelFbos.push_back(fbo);
			multiLevelTextures.push_back(texture);	
		
		}

		
		initializeShader = ShaderManager::instance()->getShader("imagePropagation");
		iniAlphaShader = ShaderManager::instance()->getShader("iniAlpha");
		copyShader = ShaderManager::instance()->getShader("copy");
		preProcessShader = ShaderManager::instance()->getShader("preProcess");
		pyramidBuilderShader = ShaderManager::instance()->getShader("pyramidBuilder");
		pyramidSmoothingShader = ShaderManager::instance()->getShader("piramidSmoothing");
		posProcessShader = ShaderManager::instance()->getShader("posProcess");
		
		debugShader = ShaderManager::instance()->getShader("debug");

		ObjModel* model;		
		model = ModelManager::instance()->getModel("plane");
		plane = new GameObject(vec3(0, 0, 0), nullptr, model);


	}

	void expandImage() {

		//clean framebuffers
		inputCreationFbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputCreationFbo->unbindCurrentFrameBuffer();
		textureCopyFbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		textureCopyFbo->unbindCurrentFrameBuffer();
		finalFbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		finalFbo->unbindCurrentFrameBuffer();
		initAlphaFbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		initAlphaFbo->unbindCurrentFrameBuffer();
		
		
		for (auto fbo : multiLevelFbos) {
			fbo->bindFrameBuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			fbo->unbindCurrentFrameBuffer();
		}


		//build foreground/background texture
		inputCreationFbo->bindFrameBuffer();
		initializeShader->use();
		initializeShader->loadImageSource(2);
		initializeShader->loadMode(mode);
		plane->draw2();
		initializeShader->unUse();
		inputCreationFbo->unbindCurrentFrameBuffer();

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, textureF);

		//build foreground/background texture
		initAlphaFbo->bindFrameBuffer();
		iniAlphaShader->use();
		iniAlphaShader->loadImageSource(10);
		plane->draw2();
		iniAlphaShader->unUse();
		initAlphaFbo->unbindCurrentFrameBuffer();

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, iniTextureF);

		

		for (int step = 0; step < iterations; step++) {

			// 5 TASKS

			// 1 - COPY INPUT TO IMAGE S
			textureCopyFbo->bindFrameBuffer();
			copyShader->use();
			copyShader->loadImageSource(10);
			plane->draw2();
			copyShader->unUse();
			textureCopyFbo->unbindCurrentFrameBuffer();
						


			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, textureS);

			//2 - PRE PROCESS IMAGE S
			multiLevelFbos[0]->bindFrameBuffer();
			preProcessShader->use();
			preProcessShader->loadImageSource(10);
			plane->draw2();
			preProcessShader->unUse();
			multiLevelFbos[0]->unbindCurrentFrameBuffer();


			//3 - BUILD PYRAMID LEVELS 0 -> MAX LEVEL
			for (int i = 0; i < levels - 1; i++) {

				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, multiLevelTextures[i]);

				FrameBuffer* fbo = multiLevelFbos[i + 1];
				fbo->bindFrameBuffer();
				pyramidBuilderShader->use();
				pyramidBuilderShader->loadImageSource(10);
				pyramidBuilderShader->loadTextureWidth((float)fbo->width * 2.0);
				pyramidBuilderShader->loadTextureHeight((float)fbo->height * 2.0);
				plane->draw2();
				pyramidBuilderShader->unUse();
				fbo->unbindCurrentFrameBuffer();


			}
			


			for (int i = 0; i < levels - 1; i++) {
				FrameBuffer* fbo = multiLevelFbos[i];
				fbo->bindFrameBuffer();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				fbo->unbindCurrentFrameBuffer();
			}
		
			

			//4 - SMOOTH PYRAMID LEVELS MAX LEVEL -> 0
			for (int i = levels - 1; i > 0; i--) {

				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, multiLevelTextures[i]);

				FrameBuffer* fbo = multiLevelFbos[i - 1];
				fbo->bindFrameBuffer();
				pyramidSmoothingShader->use();
				pyramidSmoothingShader->loadImageSource(10);
				pyramidSmoothingShader->loadTextureWidth((float)fbo->width / 2.0);
				pyramidSmoothingShader->loadTextureHeight((float)fbo->height / 2.0);
				plane->draw2();
				pyramidSmoothingShader->unUse();
				fbo->unbindCurrentFrameBuffer();

			}

			

			

			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, textureF);
			glActiveTexture(GL_TEXTURE11);
			glBindTexture(GL_TEXTURE_2D, multiLevelTextures[0]);

			////5 - POS PROCESS IMAGE S
			finalFbo->bindFrameBuffer();
			posProcessShader->use();
			posProcessShader->loadImageSource(10);
			posProcessShader->loadImageSource2(11);
			posProcessShader->loadIteration(step);
			plane->draw2();
			posProcessShader->unUse();
			finalFbo->unbindCurrentFrameBuffer();

			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, finalTextureF);

		}
		

		glActiveTexture(GL_TEXTURE17);
		glBindTexture(GL_TEXTURE_2D, iniTextureF);

		debugShader->use();
		debugShader->loadImageSource(17);
		plane->draw2();
		debugShader->unUse();

		glActiveTexture(GL_TEXTURE0 + outputImage);
		glBindTexture(GL_TEXTURE_2D, finalTextureF);

	

			
	
	}
	


};