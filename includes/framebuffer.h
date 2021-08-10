#pragma once
#include <glad/glad.h>
#include <iostream>


struct FrameBuffer {
	unsigned int rendererID;
	//depth buffer
	GLuint depthBuffer;
	// The texture we're going to render to
	GLuint renderedTexture;

	FrameBuffer();

	void bind(const int slot = 2) const;

	void Unbind() const;

	int createColorTexture(const int width, const int height);

	int createDepthTexture(const int width, const int height);
};
