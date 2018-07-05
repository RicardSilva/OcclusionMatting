#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
class FrameBuffer {
protected:
	int WIDTH;
	int HEIGHT;

	GLuint frameBuffer;
	GLuint colorTexture;
	GLuint depthTexture;


public:
	FrameBuffer(int width, int height) : WIDTH(width), HEIGHT(height) {
		frameBuffer = createFrameBuffer();
		colorTexture = createTextureAttachment(width, height);
		depthTexture = createDepthTextureAttachment(width, height);
		unbindCurrentFrameBuffer();
	}

	FrameBuffer(int width, int height, int a) : WIDTH(width), HEIGHT(height) {
		
	}

	virtual ~FrameBuffer() {
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &colorTexture);
		glDeleteTextures(1, &depthTexture);
	}

	void bindFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, WIDTH, HEIGHT);
	}
	void unbindCurrentFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
	}

	int getColorTexture() {
		return colorTexture;
	}
	int getDepthTexture() {
		return depthTexture;
	}

protected:	



	int createFrameBuffer() {
		GLuint frameBuffer;
		glGenFramebuffers(1, &frameBuffer);
		//generate name for frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		//create the framebuffer
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		//indicate that we will always render to color attachment 0
		return frameBuffer;
	}

	virtual int createTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			texture, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	}
	int createDepthTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			texture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	}

};
