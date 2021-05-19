#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <string>
#include <iostream>

struct Textures {
	unsigned int textureID;
	std::string filePath;
	int height, width;
	unsigned char* data;

	Textures(std::string path);

	void bind(const int slot = 0) const;

	void unbind();

	~Textures();
};