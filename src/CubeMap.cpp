#include "..\includes\CubeMap.h"

CubeMap::CubeMap(std::vector<std::string> faces):texture(GL_TEXTURE_CUBE_MAP){
	texture.bind();
	textureID = texture.textureID;

	for (int i = 0; i < faces.size(); i++) {
		texture.loadTexture(faces[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
	}
}

void CubeMap::bind() const{
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}
