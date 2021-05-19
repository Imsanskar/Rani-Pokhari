#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Shader {
	std::string vertexSource;
	std::string fragmentSource;
	std::string filePath;
	unsigned int rendererID;
	std::map < std::string, int> uniformStorage;

	Shader(std::string _filePath);

	void parseShader();

	int compileShader(std::string shaderSource, unsigned int type) const;

	unsigned int CreateShaderProgram();

	void bind() const;

	void unbind() const;

	void setUniform(std::string name, float value);
	
	void setUniform(std::string name, int value);

	void setUniform(std::string name, float v0, float v1, float v2, float v3);
	
	void setUniform(std::string name, glm::mat4 transform);

	int GetUniformLocation(std::string& name);

	~Shader();
};