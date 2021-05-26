#include <Textures.h>


Texture::Texture(const unsigned int _target){
	glGenTextures(1, &textureID);
	target = _target;
}

void Texture::loadTexture(std::string filePath, GLenum texTarget){
	//glGenTextures(1, &textureID);
	glBindTexture(target, textureID);

	int nrChannels;
	int height, width;
	data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(texTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		if(target == GL_TEXTURE_2D)
			glGenerateMipmap(target);
	}
	else {
		std::cout << "Failed to load texture " << filePath <<std::endl;
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//free the data
	stbi_image_free(data);
}

Texture::Texture(std::string filePath, const unsigned int _target):textureID(0), target(_target){
	loadTexture(filePath);
}

void Texture::bind(const int slot) const{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(target, textureID);
}

void Texture::unbind(){
	glBindTexture(target, 0);
}

Texture::~Texture(){
	glDeleteTextures(1, &textureID);
}
