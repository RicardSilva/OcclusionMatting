#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
class FrameBuffer {

	int WIDTH = 1200;
	int HEIGHT = 900;

	GLuint frameBuffer;
	GLuint colorTexture;
	GLuint depthTexture;


public:
	FrameBuffer() {
		frameBuffer = createFrameBuffer();
		colorTexture = createTextureAttachment(WIDTH, HEIGHT);
		depthTexture = createDepthTextureAttachment(WIDTH, HEIGHT);
		unbindCurrentFrameBuffer();
	}

	virtual ~FrameBuffer() {
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &colorTexture);
		glDeleteTextures(1, &depthTexture);
	}

	void bindFrameBuffer() {
		bindFrameBuffer(frameBuffer, WIDTH, HEIGHT);
	}
	void unbindCurrentFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int getColorTexture() {
		return colorTexture;
	}
	int getDepthTexture() {
		return depthTexture;
	}

private:	

	void bindFrameBuffer(int frameBuffer, int width, int height) {
		glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	}

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

	int createTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
			0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			texture, 0);
		glBindTexture(GL_TEXTURE_2D, texture);
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
		return texture;
	}

};
