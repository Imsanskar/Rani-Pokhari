#include <Textures.h>


Texture::Texture(const unsigned int _target){
	glGenTextures(1, &textureID);
	target = _target;
	format = GL_RGB;
}

Texture::Texture(std::string path, std::string _type){
	type = _type;
	loadTexture(path);
	this->path = path;
}

void Texture::loadTexture(std::string filePath, GLenum texTarget){
	//glGenTextures(1, &textureID);
	path = filePath;
	glBindTexture(target, textureID);

	int nrChannels;
	int height, width;
	data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(texTarget, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		if(target == GL_TEXTURE_2D)
			glGenerateMipmap(target);
	}
	else {
		std::cout << "Failed to load texture " << filePath <<std::endl;
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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

void Texture::unbind() const{
	glBindTexture(target, 0);
}

Texture::~Texture(){
//	glDeleteTextures(1, &textureID);
}
