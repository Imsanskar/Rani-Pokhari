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


int main() {
	Renderer renderer;
	
	glfwInit();
	
	const int width = 1800, height = 1200;
	float aspectRatio = float(width) / float(height);
	//GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl Project", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl Project", NULL, NULL);
	
	//disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	//mouse movement call back
	glfwSetCursorPosCallback(window, mouseCallback);
	
	glfwSetScrollCallback(window, scrollCallBack);
	
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	UserContext context;
	
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	float c = 0.5f;
	
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
	Shader shader("../../../resources/shaders/main.shader");
	Shader lampShader("../../../resources/shaders/lamp.glsl");


	VertexArray va;
	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	va.addBuffer(vertexBuffer, layout);

	Texture water("../../../resources/textures/water.jpg");
	glCheckError(water.bind(0));
	
	
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
	
	shader.bind();
	//projection matrix
	glm::mat4 projection;
	float fov = 45.0f;
	projection = glm::perspective(glm::radians(fov), (float)width / (float) height, 0.0001f, 100.0f);
	shader.setUniform("projection", projection);
	
	//skybox shader uniform
	skyBoxShader.bind();
	shader.setUniform("projection", projection);
	
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
	//Model ictc("../../../resources/models/car/Lamborghini_Aventador.fbx");
	Model ictc("../../../resources/models/Gajur/Gajur.obj");

	while (!glfwWindowShouldClose(window)) {
		renderer.clear(1.0f, 0.0f, 1.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		glDepthMask(GL_FALSE);
		skyBoxShader.bind();
		glm::mat4 view = glm::mat4(glm::mat3(renderer.camera.GetLookAtMatrix()));
		skyBoxShader.setUniform("view", view);
		skyBoxShader.setUniform("projection", projection);
		skyBox.bind();
		renderer.draw(skyBoxVA, skyBoxShader, 36);
		glDepthMask(GL_TRUE);
		
		
		shader.bind();
		float timeValue = (float)glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		float redValue = (cos(timeValue) / 2.0f) + 0.5f;
		float blueValue = sin(timeValue) / 2.0f + 0.5f;
		
		shader.setUniform("view", renderer.camera.GetLookAtMatrix());
		
		view = renderer.camera.GetLookAtMatrix();
		projection = glm::perspective(glm::radians(context.fov), aspectRatio, 0.1f, 1000.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)timeValue, glm::vec3(0.0f, 1.0f, 0.0f));
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[1]);
		float angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));


		//set uniforms
		//shader.setUniform("transform", trans);
		shader.setUniform("model", model);
		shader.setUniform("projection", projection);
		shader.setUniform("view", view);
		/*
		shader.setUniform("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setUniform("objectColor", 1.0f, 0.5f, 0.31f);
		*/
		shader.setUniform("lightPos", cubePositions[0]);
		shader.setUniform("viewPos", camera.cameraPosition);
		shader.setUniform("material.ambient", 0.0f, 0.5f, 0.31f);
		shader.setUniform("material.diffuse", 1.0f, 0.5f, 0.31f);
		shader.setUniform("material.specular", 1.0f, 0.5f, 0.5f);
		shader.setUniform("material.shininess", 32.0f);
		shader.setUniform("light.ambient", 0.8f, 0.2f, 0.2f);
		shader.setUniform("light.diffuse", 1.0f, 0.5f, 0.5f); // darken diffuse light a bit
		shader.setUniform("light.specular", 1.0f, 1.0f, 1.0f);
		//renderer.draw(va, shader, 36);

		lampShader.bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[0]);
		lampShader.setUniform("model", model);
		lampShader.setUniform("projection", projection);
		lampShader.setUniform("view", view);

		//renderer.draw(va, lampShader, 36);

		modelShader.bind();
		trans = glm::mat4(0.5f);
		//trans = glm::rotate(trans, (float)timeValue, glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::scale(trans, glm::vec3(0.2f, 0.2f, 0.2f));
		trans = glm::translate(trans, glm::vec3(0.0f, -0.4f, 0.0f));
		glCheckError(modelShader.setUniform("trans", trans));
		glCheckError(modelShader.setUniform("model", model));
		glCheckError(modelShader.setUniform("projection", projection));
		glCheckError(modelShader.setUniform("view", view));
		modelShader.setUniform("viewPos", camera.cameraPosition);
		modelShader.setUniform("light.ambient", 0.4f, 0.4f, 0.2f);
		modelShader.setUniform("light.diffuse", 1.0f, 1.0f, 1.0f); // darken diffuse light a bit
		modelShader.setUniform("light.specular", 1.0f, 1.0f, 1.0f);
		glCheckError(ictc.render(modelShader));

		glfwSwapBuffers(window);
		glfwPollEvents();
		
		renderer.processKeyboardInput(window);
	}
}
