#include "Shaders.h"


Shader::Shader(std::string _filePath):filePath(_filePath){
	parseShader();

	rendererID = CreateShaderProgram();
}

void Shader::parseShader(){
	std::ifstream stream;
	stream.open(filePath);
	enum class ShaderType {
		NONE = 0,
		VERTEX = 1,
		FRAGMENT = 2
	};

	ShaderType type = ShaderType::NONE;

	std::string line;
	std::stringstream ss[3];

	while (std::getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}
	vertexSource = ss[1].str();
	fragmentSource = ss[2].str();
}



int Shader::compileShader(std::string shaderSource, unsigned int type) const {
	unsigned int id = glCreateShader(type);

	const char* src = shaderSource.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "Failed to compile " << "File: " << filePath <<
			(type == GL_VERTEX_SHADER ? " vertex" : " fragment") << " shader\n";
		std::cout << infoLog << std::endl;
		glDeleteShader(id);
		exit(-1);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShaderProgram(){
	unsigned int shaderProgram = glCreateProgram();

	unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void Shader::bind() const{
	glUseProgram(rendererID);
}

void Shader::unbind() const{
	glUseProgram(0);
}

void Shader::setUniform(std::string name, float value){
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::setUniform(std::string name, int value){
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::setUniform(std::string name, float v0, float v1, float v2, float v3){
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform(std::string name, float v0, float v1, float v2) {
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}


void Shader::setUniform(std::string name, MathLib::vec3 vec) {
	glUniform3fv(GetUniformLocation(name),1, vec.value_ptr());
}


void Shader::setUniform(std::string name, MathLib::mat4 transform){
	glUniformMatrix4fv(GetUniformLocation(name), 1, false, transform.value_ptr());
}

int Shader::GetUniformLocation(std::string& name){
	if (uniformStorage.find(name) != uniformStorage.end()) {
		return uniformStorage[name];
	}
	int location = glGetUniformLocation(rendererID, name.c_str());
	if (location == -1)
		std::cerr << "Uniform " << name << " does not exist." << " File path: " << filePath << "\n";
	uniformStorage[name] = location;
	return location;
}

Shader::~Shader(){
	glDeleteProgram(rendererID);
}

int CompileAndLogShader(const std::string& path, GLuint shader_type)
{
    std::ifstream read_file {path.data(), std::ios::in};
    if (read_file.fail())
    {
	std::cout << "Failed to open shader file " << path.data() << std::endl;
	return -1;
    }

    std::string temp;
    std::stringstream content;
    while (std::getline(read_file,temp))
	content << temp << '\n';

    auto shader = glCreateShader(shader_type);
    auto str = content.str();
    auto c_str = str.c_str();
    glShaderSource(shader,1,&c_str,NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if (!success)
    {
	char infoLog[512];
	glGetShaderInfoLog(shader,512,nullptr,infoLog);
	std::cerr << "\nFailed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << infoLog << std::endl;
	return -1;
    }
    else
	std::cerr << '\n' << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compilation passed." << std::endl;
    return shader;
}