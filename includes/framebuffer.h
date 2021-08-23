#pragma once
#include <glad/glad.h>
#include <iostream>
#include "Renderer.h"


struct FrameBuffer {
	unsigned int rendererID;
	//depth buffer
	GLuint depthBuffer;
	GLuint depthTexture;
	// The texture we're going to render to
	GLuint renderedTexture;

	FrameBuffer();

	void bind(const int widht, const int height) const;

	void Unbind() const;

	int createColorTexture(const int width, const int height);

	int createDepthTexture(const int widht, const int height);

	int createDepthBufferAttachment(const int width, const int height);
};
