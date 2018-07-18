#pragma once
#include "FrameBuffer.h"


class MultipleLevelsFrameBuffer : public FrameBuffer {
	GLuint texture;
	int level = 0;

public:

	MultipleLevelsFrameBuffer(int width, int height) : FrameBuffer(width, height, 1) {
		frameBuffer = createFrameBuffer();
		colorTexture = createTextureAttachment(width, height);
		depthTexture = createDepthTextureAttachment(width, height);
		unbindCurrentFrameBuffer();
	
	}

	MultipleLevelsFrameBuffer(int width, int height, GLuint texture, int level) : texture(texture), level(level), FrameBuffer(width, height, 1) {
		frameBuffer = createFrameBuffer();
		colorTexture = createTextureAttachment(width, height, texture, level);
		depthTexture = createDepthTextureAttachment(width, height);
		unbindCurrentFrameBuffer();

	}


	int createTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glGenerateMipmap(GL_TEXTURE_2D);
	/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			texture, 0);


		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;

	}

	int createTextureAttachment(int width, int height, GLuint texture, int level) {
		/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
		glBindTexture(GL_TEXTURE_2D, texture);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, level);


		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;

	}
		
	

};