#include <glad/glad.h>
#include "framebuffer.h"

struct WaterFrameBuffer{
	FrameBuffer reflectionFrameBuffer;
	
	FrameBuffer refractionFrameBuffer;

	int reflectionHeight;
	int reflectionWidth;


	int refractionHeight;
	int refractionWidth;
	
	WaterFrameBuffer(){
		reflectionHeight = Renderer::height;
		reflectionWidth = Renderer::width;

		refractionHeight = 1200;
		refractionWidth = 720;	
	}

	void initReflectionFrameBuffer(){
		reflectionFrameBuffer.bind(reflectionWidth, reflectionHeight);
		reflectionFrameBuffer.createColorTexture(reflectionWidth, reflectionHeight); 	
		reflectionFrameBuffer.createDepthBufferAttachment(reflectionWidth, reflectionHeight);
	}


	void initRefractionFrameBuffer(){
		refractionFrameBuffer.createColorTexture(refractionWidth, refractionHeight); 	
		refractionFrameBuffer.createDepthTexture(refractionWidth, refractionHeight);
	}

	void unbind(){
		reflectionFrameBuffer.Unbind();
		refractionFrameBuffer.Unbind();
	}

	void bindReflectionFrameBuffer(){
		reflectionFrameBuffer.bind(reflectionWidth, reflectionHeight);
	}

	void bindRefractionFrameBuffer(){
		refractionFrameBuffer.bind(refractionWidth, refractionHeight);
	}
};