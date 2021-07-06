#include <glad/glad.h>
#include <iostream>
#include "Renderer.h"
#include <glm/glm.hpp>
#include <Textures.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <CubeMap.h>
#include <Model.h>
#include <tchar.h>
#include <windows.h>
#include <Lmcons.h>


extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}



int main() {
	Renderer renderer;

	glfwInit();

	const int width = 1920, height = 1400;
	float aspectRatio = float(width) / float(height);
	//GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl Project", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl Project", NULL, NULL);

	//disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//mouse movement call back
	glfwSetCursorPosCallback(window, mouseCallback);

	//scroll callback
	glfwSetScrollCallback(window, scrollCallBack);

	//window resize call back
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	//context for the glfw window
	UserContext context;


	//get user name
	DWORD  bufCharCount = 200;
	char* infoBuf = (char*)malloc(sizeof(char) * bufCharCount);
	GetUserName((LPWSTR)infoBuf, &bufCharCount);
	_tprintf(TEXT("User name: %s\n"), infoBuf);


	//open gl settings
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
	const GLubyte* rendererData = glGetString(GL_RENDERER); // Returns a hint to the model
	std::cout << "Vendor:" << vendor << "   " << "Graphics card:" << rendererData << std::endl;

	if (strcmp(infoBuf, "imsanskar") == 0) {
		std::string str((char*)vendor);
		if (str.find("NVIDIA") == std::string::npos) {
			std::cout << "Use nvidia card unless you want to get bsod";
			exit(-1);
		}
	}

	float c = 0.5f;

	//For blending, i.e. for textures with RGBA values
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float data[] = {
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,


		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f
	};

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		//glm::vec3(-1.5f, -2.2f, -2.5f),
		//glm::vec3(-3.8f, -2.0f, -12.3f),
		//glm::vec3(2.4f, -0.4f, -3.5f),
		//glm::vec3(-1.7f,  3.0f, -7.5f),
		//glm::vec3(1.3f, -2.0f, -2.5f),
		//glm::vec3(1.5f,  2.0f, -2.5f),
		//glm::vec3(1.5f,  0.2f, -1.5f),
		//glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
		1, 2, 3    // second triangle
	};

	VertexBuffer vertexBuffer(data, sizeof(data));
	IndexBuffer indexBuffer(indices, sizeof(indices));
	Shader lightShader("../../../resources/shaders/light.glsl");
	Shader lampShader("../../../resources/shaders/lamp.glsl");
	

	VertexArray va;
	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	va.addBuffer(vertexBuffer, layout);

	//Texture water("../../../resources/textures/water.jpg");
	//glCheckError(water.bind());


	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	const CubeMap skyBox(std::vector<std::string>{
		"../../../resources/skybox/right.jpg",
			"../../../resources/skybox/left.jpg",
			"../../../resources/skybox/top.jpg",
			"../../../resources/skybox/bottom.jpg",
			"../../../resources/skybox/front.jpg",
			"../../../resources/skybox/back.jpg"});

	Shader skyBoxShader("../../../resources/shaders/cubeMap.shader");
	skyBoxShader.bind();
	VertexBuffer skyBoxVB(skyboxVertices, sizeof(skyboxVertices));
	VertexBufferLayout skyBoxLayout;
	skyBoxLayout.push<float>(3);
	VertexArray skyBoxVA;
	skyBoxVA.addBuffer(skyBoxVB, skyBoxLayout);

	
	//projection matrix
	glm::mat4 projection;
	float fov = 45.0f;
	projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.0001f, 100.0f);
	
	//skybox shader uniform
	skyBoxShader.bind();
	
	//camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Camera camera(cameraPos, cameraFront, cameraUp);
	renderer.camera = camera;

	glEnable(GL_DEPTH_TEST);

	context.renderer = &renderer;
	context.fov = fov;
	glfwSetWindowUserPointer(window, &context);


	//model
	Shader modelShader("../../../resources/shaders/model.glsl");

	Model ictc("../../../resources/models/Temple/rani final.obj");


	while (!glfwWindowShouldClose(window)) {
		//renderer.clear(0.6f, 0.8f, 0.8f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);
		//skybox
		skyBoxShader.bind();
		glm::mat4 view = glm::mat4(glm::mat3(renderer.camera.GetLookAtMatrix()));
		skyBoxShader.setUniform("view", view);
		skyBoxShader.setUniform("projection", projection);
		skyBox.bind();
		renderer.draw(skyBoxVA, skyBoxShader, 36);
		glDepthMask(GL_TRUE);
		skyBoxShader.unbind();

		//setting for models
		float angle = 0.0f;
		float timeValue = (float)glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		float redValue = (cos(timeValue) / 2.0f) + 0.5f;
		float blueValue = sin(timeValue) / 2.0f + 0.5f;
		view = renderer.camera.GetLookAtMatrix();
		projection = glm::perspective(glm::radians(context.fov), aspectRatio, 0.1f, 1000.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)timeValue, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[0]);
		
		

		////property of sun or lamp
		//lampShader.bind();
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, cubePositions[1]);
		//lampShader.setUniform("model", model);
		//lampShader.setUniform("projection", projection);
		//lampShader.setUniform("view", view);
		//lampShader.setUniform("lightColour", 1.0f,1.0f,1.0f);
		//lampShader.unbind();
		////renderer.draw(va, lampShader, 36);

		//model loading
		model = glm::translate(model, cubePositions[0]);
		modelShader.bind();		
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, -0.5f, 0.5f));
		angle = 0.0f;
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.0f, -2.5f, -2.0f));
		trans = glm::scale(trans, glm::vec3(100.5f, 100.5f, 100.5f));
		//modelShader.setUniform("time", timeValue);
		modelShader.setUniform("trans", trans);
		modelShader.setUniform("model", model);
		modelShader.setUniform("projection", projection);
		modelShader.setUniform("view", view);
		//ictc.render(modelShader, false);
		glCheckError(ictc.render(modelShader, false));
		modelShader.unbind();


		//ligtening
		lightShader.bind();
		model = glm::translate(model, cubePositions[0]);
		lightShader.setUniform("model", model);
		lightShader.setUniform("projection", projection);
		lightShader.setUniform("view", view);
		//property of light
		lightShader.setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
		lightShader.setUniform("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		lightShader.setUniform("light.specular", 1.0f, 1.0f, 1.0f);
		lightShader.setUniform("light.position", 0.0f, 0.0f, 12.0f);
		//property of material
		lightShader.setUniform("material.ambient", 0.5f, 0.5f, 0.5f);
		lightShader.setUniform("material.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.setUniform("material.specular", 0.5f, 0.5f, 0.5f);
		lightShader.setUniform("material.shininess", 32.0f);
		lightShader.setUniform("viewPos", camera.cameraPosition);
		lightShader.unbind();


		glfwSwapBuffers(window);
		glfwPollEvents();
		renderer.processKeyboardInput(window);

	}

}
