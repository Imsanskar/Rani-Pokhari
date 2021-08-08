#pragma once
#include <glad/glad.h>


struct FrameBuffer {
	unsigned int rendererID;

	FrameBuffer();

	~FrameBuffer();

	void bind() const;

	void Unbind() const;
};
