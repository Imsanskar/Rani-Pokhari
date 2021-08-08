#include "framebuffer.h"

FrameBuffer::FrameBuffer(){
	glGenBuffers(1, &rendererID);
}


void FrameBuffer::bind() const{
	glBindBuffer(GL_FRAMEBUFFER, rendererID);
}

void FrameBuffer::Unbind() const{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
} 