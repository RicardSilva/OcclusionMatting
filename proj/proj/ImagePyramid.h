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

	GLuint textureF;
	GLuint textureS;
	std::vector<GLuint> multiLevelTextures;
	std::vector<FrameBuffer*> multiLevelFbos;
	GLuint finalTextureF;
	GLuint propagationCosts;



	FrameBuffer* initialFbo;
	FrameBuffer* finalFbo;

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

		
		initialFbo = new FrameBuffer(1024, 1024);
		textureF = initialFbo->getColorTexture();

		/*glGenTextures(1, &textureF);
		glBindTexture(GL_TEXTURE_2D, textureF);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1024, 1024,
			0, GL_RGBA, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindImageTexture(0, textureF, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);*/


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

		//glBindImageTexture(1, multiLevelTextures[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		glGenTextures(1, &finalTextureF);
		glBindTexture(GL_TEXTURE_2D, finalTextureF);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1024, 1024,
			0, GL_RGBA, GL_FLOAT, 0);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindImageTexture(2, finalTextureF, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

		glGenTextures(1, &propagationCosts);
		glBindTexture(GL_TEXTURE_2D, propagationCosts);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 1024, 1024,
			0, GL_RED, GL_FLOAT, 0);



		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindImageTexture(3, propagationCosts, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);
		
		initializeShader = ShaderManager::instance()->getShader("imagePropagation");
		copyShader = ShaderManager::instance()->getShader("copy");
		copyShader->use();
		copyShader->loadInputTexture(10);
		copyShader->loadOutputImage(2);
		copyShader->unUse();

		preProcessShader = ShaderManager::instance()->getShader("preProcess");
		pyramidBuilderShader = ShaderManager::instance()->getShader("pyramidBuilder");
		pyramidBuilderShader->use();
		pyramidBuilderShader->loadInputTexture(10);
		pyramidBuilderShader->unUse();

		pyramidSmoothingShader = ShaderManager::instance()->getShader("piramidSmoothing");
		pyramidSmoothingShader->use();
		pyramidSmoothingShader->loadInputTexture(10);
		pyramidSmoothingShader->unUse();

		posProcessShader = ShaderManager::instance()->getShader("posProcess");
		
		debugShader = ShaderManager::instance()->getShader("debug");

		ObjModel* model;		
		model = ModelManager::instance()->getModel("plane");
		plane = new GameObject(vec3(0, 0, 0), nullptr, model);


	}

	void expandImage() {

		initialFbo->bindFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		initialFbo->unbindCurrentFrameBuffer();

		//glBindImageTexture(0, textureF, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		//glBindImageTexture(1, multiLevelTextures[0], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(2, finalTextureF, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glBindImageTexture(3, propagationCosts, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);
		
		//build foreground/background texture
		glViewport(0, 0, 1024, 1024);
		initialFbo->bindFrameBuffer();
		initializeShader->use();
		initializeShader->loadInputTexture(2);
		initializeShader->loadMode(mode);
		initializeShader->loadOutputImage(0);
		plane->draw2();
		initializeShader->unUse();
		initialFbo->unbindCurrentFrameBuffer();
		
	
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, textureF);
		
	
		for (int step = 0; step < iterations; step++) {

			glClearColor(0, 0, 0, 0);

			for (auto fbo : multiLevelFbos) {
				fbo->bindFrameBuffer();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				fbo->unbindCurrentFrameBuffer();
			}
			glViewport(0, 0, 1024, 1024);
			// 5 TASKS

			// 1 - COPY INPUT TO IMAGE S 
			//AND 
			//2 - INITIALIZE ALPHA VALUES
			multiLevelFbos[0]->bindFrameBuffer();
			copyShader->use();
			copyShader->loadInputTexture(10);
			copyShader->loadOutputImage(1);
			plane->draw2();
			copyShader->unUse();
			multiLevelFbos[0]->unbindCurrentFrameBuffer();
	
						
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			pyramidBuilderShader->use();
			FrameBuffer* fbo;
			//3 - BUILD PYRAMID LEVELS 0 -> MAX LEVEL
			for (int i = 0; i < levels - 1; i++) {

				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, multiLevelTextures[i]);

				fbo = multiLevelFbos[i + 1];
				fbo->bindFrameBuffer();				
				pyramidBuilderShader->loadTextureWidth(fbo->width );
				pyramidBuilderShader->loadTextureHeight(fbo->height );
				plane->draw2();
			
				fbo->unbindCurrentFrameBuffer();

			}
			pyramidBuilderShader->unUse();

			
		
			for (int i = 0; i < levels - 1; i++) {
				fbo = multiLevelFbos[i];
				fbo->bindFrameBuffer();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				fbo->unbindCurrentFrameBuffer();
			}
				
			glViewport(0, 0, 1024, 1024);
				
			//4 - SMOOTH PYRAMID LEVELS MAX LEVEL -> 0
			pyramidSmoothingShader->use();
			for (int i = levels - 1; i > 0; i--) {

				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, multiLevelTextures[i]);

				fbo = multiLevelFbos[i - 1];
				fbo->bindFrameBuffer();			
				pyramidSmoothingShader->loadTextureWidth(fbo->width);
				pyramidSmoothingShader->loadTextureHeight(fbo->height);
				plane->draw2();				
				fbo->unbindCurrentFrameBuffer();

			}

			pyramidSmoothingShader->unUse();
			glViewport(0, 0, 1024, 1024);
			
			

			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, textureF);
			glActiveTexture(GL_TEXTURE11);
			glBindTexture(GL_TEXTURE_2D, multiLevelTextures[0]);

	
			//////5 - POS PROCESS IMAGE S
			posProcessShader->use();
			posProcessShader->loadInputTexture(10);
			posProcessShader->loadInputTexture2(11);
			posProcessShader->loadOutputImage(2);
			posProcessShader->loadOutputImage2(3);
			posProcessShader->loadIteration(step);
			posProcessShader->loadTextureWidth(1024);
			posProcessShader->loadTextureHeight(1024);
			plane->draw2();
			posProcessShader->unUse();
			glViewport(0, 0, 1024, 1024);
			
			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, finalTextureF);

		}

		


		glActiveTexture(GL_TEXTURE0 + outputImage);
		glBindTexture(GL_TEXTURE_2D, finalTextureF);
	
		glViewport(0, 0, 1920, 1080);
		
	}
	


};