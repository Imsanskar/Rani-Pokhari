#include <glad/glad.h>
#include <iostream>
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
	std::string username;
	DWORD username_len = UNLEN + 1;
	// int checkUserName = GetUserName((LPSTR)username.c_str(), &username_len);

	printf("%s\n", username.c_str());
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

	if (true) {
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



	MathLib::vec3 cubePositions[] = {
		MathLib::vec3(0.0f,  0.0f,  0.0f),
		MathLib::vec3(2.0f,  5.0f, -15.0f),
	};


	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
		1, 2, 3    // second triangle
	};

	Shader lightShader("../resources/shaders/light.glsl");
	Shader lampShader("../resources/shaders/lamp.glsl");
	Shader lightning("../resources/shaders/lightmap.glsl");


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
		"../resources/skybox/right.jpg",
			"../resources/skybox/left.jpg",
			"../resources/skybox/top.jpg",
			"../resources/skybox/bottom.jpg",
			"../resources/skybox/front.jpg",
			"../resources/skybox/back.jpg"});

	Shader skyBoxShader("../resources/shaders/cubeMap.shader");
	skyBoxShader.bind();
	VertexBuffer skyBoxVB(skyboxVertices, sizeof(skyboxVertices));
	VertexBufferLayout skyBoxLayout;
	skyBoxLayout.push<float>(3);
	VertexArray skyBoxVA;
	skyBoxVA.addBuffer(skyBoxVB, skyBoxLayout);

	
	//projection matrix
	MathLib::mat4 projection;
	float fov = 45.0f;
	projection = MathLib::perspective(to_radians(fov), (float)width / (float)height, 0.0001f, 100.0f);
	
	//skybox shader uniform
	skyBoxShader.bind();
	
	//camera
	MathLib::vec3 cameraPos = MathLib::vec3(0.0f, 0.0f, 3.0f);
	MathLib::vec3 cameraFront = MathLib::vec3(0.0f, 0.0f, -1.0f);
	MathLib::vec3 cameraUp = MathLib::vec3(0.0f, 1.0f, 0.0f);
	Camera camera(cameraPos, cameraFront, cameraUp);
	renderer.camera = camera;

	glEnable(GL_DEPTH_TEST);

	context.renderer = &renderer;
	context.fov = fov;
	glfwSetWindowUserPointer(window, &context);


	//model
	Shader modelShader("../resources/shaders/model.glsl");


	//Model ictc("../resources/models/Temple/rani final.obj");
	Model temple("../resources/models/Raanipokhari.obj");
	Model sun("../resources/models/sun.obj");
	

	while (!glfwWindowShouldClose(window)) {
		//renderer.clear(0.6f, 0.8f, 0.8f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);
		//skybox
		skyBoxShader.bind();
		MathLib::mat4 view = MathLib::mat4(MathLib::mat3(renderer.camera.GetLookAtMatrix()));
		skyBoxShader.setUniform("view", view);
		skyBoxShader.setUniform("projection", projection);
		skyBox.bind();
		// renderer.draw(skyBoxVA, skyBoxShader, 36);
		glDepthMask(GL_TRUE);
		skyBoxShader.unbind();

		//setting for models
		float angle = 0.0f;
		float timeValue = (float)glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		float redValue = (cos(timeValue) / 2.0f) + 0.5f;
		float blueValue = sin(timeValue) / 2.0f + 0.5f;
		view = renderer.camera.GetLookAtMatrix();
		projection = MathLib::perspective(to_radians(context.fov), aspectRatio, 0.1f, 1000.0f);
		MathLib::mat4 trans = MathLib::mat4(1.0f);
		MathLib::mat4 model = MathLib::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		model = MathLib::translate(model, cubePositions[0]);
		model = MathLib::mat4(1.0f);
		lightning.bind();
		model = MathLib::rotate(model, to_radians(angle), MathLib::vec3(0.5f, -0.5f, 0.5f));
		angle = 0.0f;
		trans = MathLib::mat4(1.0f);
		trans = MathLib::translate(trans, MathLib::vec3(0.0f, -2.5f, -2.0f));
		trans = MathLib::scale(trans, MathLib::vec3(00.05f, 00.05f, 00.05f));
		
	
		float pt = int(timeValue) % 45*4;//converted 45 sec tie value to 180 degree to be use in light direction
		MathLib::vec3 sunpos = MathLib::vec3(0.0f, 10.0f, 0.0f); 			
		model = MathLib::translate(model, MathLib::vec3(0.0f, 3.0f, 0.0f));
		lightning.setUniform("trans", trans);
		lightning.setUniform("model", model);
		lightning.setUniform("projection", projection);
		lightning.setUniform("view", view);
		lightning.setUniform("material.shininess", 32.0f);
		lightning.setUniform("viewPos", camera.cameraPosition);
		lightning.setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		lightning.setUniform("light.specular", 1.0f, 1.0f, 1.0f);
		lightning.setUniform("light.position", sunpos);

		//ictc.render(modelShader, false);
		glCheckError(temple.render(lightning, true));
		lightning.unbind();

		//////property of sun or lamp
		lampShader.bind();
		MathLib::mat4 modelSun = MathLib::mat4(1.0f);
		modelSun = MathLib::translate(modelSun, sunpos);
		modelSun = MathLib::scale(modelSun, MathLib::vec3(0.2f));
		MathLib::mat4 transformationSun = MathLib::mat4(1.0f);
		lampShader.setUniform("model", modelSun);
		lampShader.setUniform("projection", projection);
		lampShader.setUniform("view", view);
		lampShader.setUniform("lightColour", 1.0f, 1.0f, 1.0f);
		glCheckError(sun.render(lampShader, true));
		lampShader.unbind();



		glfwSwapBuffers(window);
		glfwPollEvents();
		renderer.processKeyboardInput(window);

	}

}
