#include <glad/glad.h>
#include <iostream>
#include <CubeMap.h>
#include <Model.h>
#include <tchar.h>
#include <windows.h>
#include <Lmcons.h>

#define SPECULAR 0.8
MathLib::vec3 sunpos = MathLib::vec3(3.0f, 10.0f, 10.0f); 			

MathLib::vec3 pointLightsPositions[] = {
	MathLib::vec3(-0.85, -2.10311, -3.187),
	MathLib::vec3(-0.848091,-2.08694,-4.82994),
	MathLib::vec3(0.839003,-2.08371,-3.10848),
	MathLib::vec3(0.838302,-2.14642,-4.85632),
	MathLib::vec3(0.0139868,-0.715353,-3.92357)
}; 

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
	context.isNightMode = false;


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

	//For blending, i.e. for textures with RGBA values
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//shader
	Shader lightShader("../resources/shaders/light.glsl");
	Shader lampShader("../resources/shaders/lamp.glsl");
	Shader lightning("../resources/shaders/lightmap.glsl");
	Shader waterShader("../resources/shaders/water.glsl");


	//vertices for skybox
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

	//cube map path
	const CubeMap skyBox(std::vector<std::string>{
		"../resources/skybox/right.jpg",
			"../resources/skybox/left.jpg",
			"../resources/skybox/top.jpg",
			"../resources/skybox/bottom.jpg",
			"../resources/skybox/front.jpg",
			"../resources/skybox/back.jpg"});


	//shader for skybox
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
	projection = MathLib::perspective(to_radians(fov), (float)width / (float)height, 0.0001f, 1000.0f);
	
	//skybox shader uniform
	skyBoxShader.bind();
	
	//camera
	MathLib::vec3 cameraPos = MathLib::vec3(0.0f, 0.0f, 12.0f);
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


	Model temple("../resources/models/Raanipokhari.obj");
	Model water("../resources/models/water.obj");
	Model sun("../resources/models/sun.obj");


	//shadow
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMap;

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
	             SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 	

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 


	float near_plane = 1.0f, far_plane = 7.5f;
	MathLib::mat4 lightProjection = MathLib::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); 

	MathLib::mat4 lightView = MathLib::lookAt(MathLib::vec3(-2.0f, 4.0f, -1.0f), 
                                  MathLib::vec3( 0.0f, 0.0f,  0.0f), 
                                  MathLib::vec3( 0.0f, 1.0f,  0.0f)); 

	MathLib::mat4 lightSpaceMatrix = lightProjection * lightView;

	while (!glfwWindowShouldClose(window)) {
		// std::cout << "Camera Position:" << renderer.camera.cameraPosition << std::endl;

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
		view = renderer.camera.GetLookAtMatrix();


		projection = MathLib::perspective(to_radians(context.fov), aspectRatio, 0.1f, 1000.0f);
		MathLib::mat4 trans = MathLib::mat4(1.0f);
		MathLib::mat4 model = MathLib::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		lightning.bind();
		model = MathLib::mat4(1.0f);
		model = MathLib::rotate(model, to_radians(angle), MathLib::vec3(0.5f, -0.5f, 0.5f));
		angle = 0.0f;
		trans = MathLib::mat4(1.0f);
		trans = MathLib::translate(trans, MathLib::vec3(0.0f, -2.5f, -2.0f));
		trans = MathLib::scale(trans, MathLib::vec3(0.5f, 0.5f, 0.5f));
		
	
		float pt = int(timeValue) % 45*4;//converted 45 sec tie value to 180 degree to be use in light direction
		model = MathLib::translate(model, MathLib::vec3(0.0f, 3.0f, 0.0f));
		lightning.setUniform("lightMatrix", model);
		lightning.setUniform("model", model * trans);
		lightning.setUniform("projection", projection);
		lightning.setUniform("view", view);
		lightning.setUniform("material.shininess", 64.0f);
		lightning.setUniform("viewPos", camera.cameraPosition);

		lightning.setUniform("isNightMode", (int)context.isNightMode);
		
		lightning.setUniform("pointLights[0].position", pointLightsPositions[0]);
		lightning.setUniform("pointLights[0].constant", 1.0f);
		lightning.setUniform("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("pointLights[0].diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("pointLights[0].specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("pointLights[0].linear", 0.7f);
		lightning.setUniform("pointLights[0].quadratic", 0.22f);

		lightning.setUniform("pointLights[1].position", pointLightsPositions[1]);
		lightning.setUniform("pointLights[1].constant", 1.0f);
		lightning.setUniform("pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("pointLights[1].diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("pointLights[1].specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("pointLights[1].linear", 0.7f);
		lightning.setUniform("pointLights[1].quadratic", 0.22f);

		
		lightning.setUniform("pointLights[2].position", pointLightsPositions[2]);
		lightning.setUniform("pointLights[2].constant", 1.0f);
		lightning.setUniform("pointLights[2].ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("pointLights[2].diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("pointLights[2].specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("pointLights[2].linear", 0.7f);
		lightning.setUniform("pointLights[2].quadratic", 0.22f);


		lightning.setUniform("pointLights[3].position", pointLightsPositions[3]);
		lightning.setUniform("pointLights[3].constant", 1.0f);
		lightning.setUniform("pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("pointLights[3].diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("pointLights[3].specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("pointLights[3].linear", 0.7f);
		lightning.setUniform("pointLights[3].quadratic", 0.22f);


		lightning.setUniform("pointLights[4].position", pointLightsPositions[4]);
		lightning.setUniform("pointLights[4].constant", 1.0f);
		lightning.setUniform("pointLights[4].ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("pointLights[4].diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("pointLights[4].specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("pointLights[4].linear", 0.7f);
		lightning.setUniform("pointLights[4].quadratic", 0.22f);

		lightning.setUniform("pointLights[3].position", pointLightsPositions[3]);
		lightning.setUniform("pointLights[3].constant", 1.0f);
		lightning.setUniform("pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("pointLights[3].diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("pointLights[3].specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("pointLights[0].linear", 0.8f);
		lightning.setUniform("pointLights[0].quadratic", 0.22f);
		
		lightning.setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("light.diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("light.specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("light.position", sunpos);

		temple.render(lightning, true);
		lightning.unbind();


		waterShader.bind();
		model = MathLib::mat4(1.0f);
		model = MathLib::rotate(model, to_radians(angle), MathLib::vec3(0.5f, -0.5f, 0.5f));
		angle = 0.0f;
		trans = MathLib::mat4(1.0f);
		trans = MathLib::translate(trans, MathLib::vec3(0.0f, -1.6f, -2.0f));
		trans = MathLib::scale(trans, MathLib::vec3(0.5f, 0.5f, 0.5f));
		waterShader.setUniform("model", model * trans);
		waterShader.setUniform("projection", projection);
		waterShader.setUniform("view", view);
		waterShader.setUniform("material.shininess", 128.0f);
		waterShader.setUniform("viewPos", camera.cameraPosition);
		waterShader.setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
		waterShader.setUniform("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		waterShader.setUniform("light.specular", SPECULAR, SPECULAR,SPECULAR);
		waterShader.setUniform("light.position", sunpos);


		glCheckError(water.render(waterShader, true));
		waterShader.unbind();

		//////property of sun or lamp
		lampShader.bind();
		MathLib::mat4 modelSun = MathLib::mat4(1.0f);
		modelSun = MathLib::translate(modelSun, sunpos);
		modelSun = MathLib::scale(modelSun, MathLib::vec3(0.25f));
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
