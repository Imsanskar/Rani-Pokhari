#include "..\includes\Mesh.h"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures, Material _material)
	:vertices(_vertices),
	 indices(_indices),
	 textures(_textures),
	 vbo(&vertices[0], vertices.size() * sizeof(Vertex)),
	 ibo(&indices[0], indices.size() * sizeof(uint32_t)),
	 material(_material)
	{	
	setupMesh();
}

void Mesh::setupMesh(){
	VertexBufferLayout layout;

	//for vertex buffer
	layout.push<float>(3);

	//normal coordinates
	layout.push<float>(3);

	//tex coordinate
	layout.push<float>(2);

	// For Vertex Tangent
	layout.push<float>(3);

	// For Vertex Bitangent
	layout.push<float>(3);

	vao.addBuffer(vbo, layout);
}


void Mesh::draw(Shader& shader, bool isTextureModel) const{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

	shader.bind();
	 if(!isTextureModel){
		 shader.setUniform("material.ambient", material.ambient);
		 shader.setUniform("material.diffuse", material.diffuse);
		 shader.setUniform("material.specular", material.specular);
		 shader.setUniform("material.shininess", 32.0f);
	 }
    for (unsigned int i = 0; i < textures.size(); ++i){
		glActiveTexture(GL_TEXTURE0 + i);
        // retrieve texture number (the N in diffuse_textureN)
		std::string number;
        const std::string& textureType = textures[i].getType();

        if (textureType == "diffuse")
            number = std::to_string(diffuseNr++);
        else if (textureType == "specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else
            continue;
        shader.setUniform(("material." + textureType + number), static_cast<int>(i));
		glCheckError(textures[i].bind(i));
	}

	glCheckError(vao.bind());
	glCheckError(vbo.bind());
	glCheckError(ibo.bind());
	glCheckError(glDrawElements(GL_TRIANGLES, ibo.count, GL_UNSIGNED_INT, NULL));
}
