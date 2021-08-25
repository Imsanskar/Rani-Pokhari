#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

struct Model {
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;


	Model(std::string path);

	void loadModel(std::string path);

	void render(Shader& shader, unsigned int zprogram,ZRender& zrender, bool isTexture = true);

private:
	void processNode(aiNode* node, const aiScene* scene);

	void processMesh(aiMesh* mesh, const aiScene* scene);

	Material loadMaterial(aiMaterial* mat);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName);
};

unsigned int TextureFromFile(const char* path, const std::string& directory);