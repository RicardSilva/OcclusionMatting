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


	int levels;
	int iterations;

	int mode;     // 0 -> foreground == real color
				  // 1 -> background == virtual color
	int outputImage;


	std::vector<GLuint> multiLevelTextures;
	std::vector<FrameBuffer*> multiLevelFbos;


	GLuint inputTexture;
	GLuint textureS;
	GLuint preProcessedTextureS;
	GLuint outputTexture;


	FrameBuffer* inputCreationFbo;
	FrameBuffer* textureCopyFbo;

	Shader* initializeShader;
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
		inputTexture = inputCreationFbo->getColorTexture();
		textureCopyFbo = new FrameBuffer(1024, 1024);
		textureS = textureCopyFbo->getColorTexture();



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


		//build foreground/background texture
		inputCreationFbo->bindFrameBuffer();
		initializeShader->use();
		initializeShader->loadImageSource(10);
		initializeShader->loadMode(mode);
		plane->draw2();
		initializeShader->unUse();
		inputCreationFbo->unbindCurrentFrameBuffer();

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, inputTexture);

		for (int step = 1; step < iterations; step++) {

			// 5 STEPS

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
			preProcessShader->loadMode(mode);
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
				pyramidSmoothingShader->loadTextureHeight((float)fbo->height * 2.0);
				plane->draw2();
				pyramidBuilderShader->unUse();
				fbo->unbindCurrentFrameBuffer();

			}

			//4 - SMOOTH PYRAMID LEVELS MAX LEVEL -> 0
			for (int i = levels - 1; i > 0; i--) {

				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, multiLevelTextures[i]);

				FrameBuffer* fbo = multiLevelFbos[i - 1];
				fbo->bindFrameBuffer();
				pyramidBuilderShader->use();
				pyramidBuilderShader->loadImageSource(10);
				pyramidBuilderShader->loadTextureWidth((float)fbo->width / 2.0);
				pyramidSmoothingShader->loadTextureHeight((float)fbo->height / 2.0);
				plane->draw2();
				pyramidBuilderShader->unUse();
				fbo->unbindCurrentFrameBuffer();

			}

			//5 - POS PROCESS IMAGE S





		}


	
	

	

			
	
	}
	


};