#pragma once
#include <Shaders.h>
#include <vector>
#include "Textures.h"
#include <VertexArray.h>
#include <Renderer.h>
#include <unordered_map>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
};

struct Material {
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
};

struct Mesh {
    const std::vector<Vertex> vertices;
    const std::vector<uint32_t> indices;
    const std::vector<Texture> textures;
    Material material;
    //const std::unordered_map<std::string, Texture>& texturesLoaded;

    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material _material);

    void setupMesh();

    void draw(Shader& shader, bool isTextureModel = true) const;
};