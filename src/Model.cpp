#include "Model.h"

Model::Model(std::string path) {
	loadModel(path);
}

void Model::loadModel(std::string path) {
	Assimp::Importer* importer = new Assimp::Importer;
	const aiScene* scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer->GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::render(Shader& shader, bool isTexture) {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader, isTexture);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		MathLib::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		// normals
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		// does the mesh contain texture coordinates
		if (mesh->mTextureCoords[0]) {
			MathLib::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;

			//// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
			//// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}
		else {
			vertex.texCoords = MathLib::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	
	Material materialProperty = loadMaterial(scene->mMaterials[mesh->mMaterialIndex]);
	meshes.emplace_back(vertices, indices, textures, materialProperty);
}

Material Model::loadMaterial(aiMaterial* mat) {
	Material material;
	aiColor3D color(0.f, 0.f, 0.f);
	float shininess;

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material.diffuse = MathLib::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material.ambient = MathLib::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material.specular = MathLib::vec3(color.r, color.b, color.g);

	return material;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	//std::vector<Texture> textures(mat->GetTextureCount(type));
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		//Texture texture(directory + '/' + str.C_Str());
		//texture.textureID = TextureFromFile(str.C_Str(), directory);
		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size(); j++) {
			if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
				textures.emplace_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.type = typeName;
			texture.loadTexture(directory + '/' + str.C_Str());
			textures.emplace_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}
