#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <Math/matrix.h>

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
	
	void setUniform(std::string name, float v0, float v1, float v2);

	void setUniform(std::string name, MathLib::vec3 vec);

	void setUniform(std::string name, MathLib::mat4 transform);

	int GetUniformLocation(std::string& name);

	~Shader();
};

int CompileAndLogShader(const std::string& path, GLuint shader_type);


void update_uniform_3f(const char* uniform_name, unsigned int program_ID, MathLib::vec3 in_vec3);
void update_uniform_1f(const char* uniform_name, unsigned int program_ID, float uniform_value);
void update_uniform_matrix_4f(const char *uniform_name, unsigned int program_ID, float* first_element_address);
void update_uniform_1i(const char *uniform_name, unsigned int program_ID, int uniform_value);