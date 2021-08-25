#pragma once
#include <Shaders.h>
#include <vector>
#include "Textures.h"
#include <VertexArray.h>
#include <Renderer.h>
#include <unordered_map>
#include "framebuffer.h"
#include "zbuffer.hpp"

struct Vertex {
    MathLib::vec3 position;
    MathLib::vec3 normal;
    MathLib::vec2 texCoords;
    // tangent
    MathLib::vec3 tangent;
    // bitangent
    MathLib::vec3 bitangent;
};

struct Material {
    MathLib::vec3 diffuse;
    MathLib::vec3 ambient;
    MathLib::vec3 specular;
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

    void draw(Shader& shader, unsigned int zprogram,ZRender& zrender, bool isTextureModel = true) const;
};