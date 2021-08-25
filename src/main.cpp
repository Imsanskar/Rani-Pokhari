#include <glad/glad.h>
#include <iostream>
#include <CubeMap.h>
#include <Model.h>
#include "WaterFrameBuffer.h"
#include "zbuffer.hpp"

#define COMPOUND_LINEAR 0.014f
#define COMPOUND_QUAD 0.007f

#define GAJUR_LINEAR 0.35f
#define GAJUR_QUAD 0.44f
#define COMPOUND_LIGHT 0.98, 0.99, 0.97 
#define GAJUR_LIGHT 1.0f, 0.255, 0.16 


#define SPECULAR 0.8
MathLib::vec3 sunpos = MathLib::vec3(30.0f, 100.0f, 500.0f); 			

MathLib::vec3 pointLightsPositions[] = {
	//door lights
	MathLib::vec3(1.06, 4.12, -2.88),//front light
	MathLib::vec3(-0.38, 4.12, -1.50),//front light
	MathLib::vec3(1.06, 4.12, -2.88),//front light
	MathLib::vec3(-1.74, 4.12, -1.50),//front light

	//compund lights

	//left side lights
	MathLib::vec3(-52.4237,3.92044,49.5237),//left first
	MathLib::vec3(-52.4237,3.92044, 26.49),//left first
	MathLib::vec3(-52.4237,3.92044,-32.42),//left first
	MathLib::vec3(-52.4237,3.92044,-55.4537),//left first

	// back face
	MathLib::vec3(-30.35,3.92044,-55.4537),//back first
	MathLib::vec3(31.74,3.92044,-55.4537),//back second
	MathLib::vec3(54.11,3.92044,-55.4537),//back third

	// right face
	MathLib::vec3(54.11,3.92044,49.5237),//right first
	MathLib::vec3(54.11,3.92044, 26.49),//right second
	MathLib::vec3(54.11,3.92044,-32.42),//right third

	//pathway light
	MathLib::vec3(-1.19,2.91,49.5237),//left first
	MathLib::vec3(0.461,2.91,49.5237),//left first

	//temple gajur light positions
	MathLib::vec3(-0.27, 7.35, -1.77),//front
	MathLib::vec3(0.82, 7.35, -2.89),//right
	MathLib::vec3(-0.26, 7.35, -4.21),//back
	MathLib::vec3(-1.52, 7.35, -1.89),//left
}; 

extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}


void setLightPosition(Shader& lightning){
	lightning.bind();
	//gajur lights
	lightning.setUniform("pointLights[0].position", pointLightsPositions[0]);
	lightning.setUniform("pointLights[0].constant", 1.0f);
	lightning.setUniform("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[0].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[0].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[0].linear", 0.7f);
	lightning.setUniform("pointLights[0].quadratic", 1.8f);

	lightning.setUniform("pointLights[1].position", pointLightsPositions[1]);
	lightning.setUniform("pointLights[1].constant", 1.0f);
	lightning.setUniform("pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[1].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[1].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[1].linear", 0.7f);
	lightning.setUniform("pointLights[1].quadratic", 1.8f);

	
	lightning.setUniform("pointLights[2].position", pointLightsPositions[2]);
	lightning.setUniform("pointLights[2].constant", 1.0f);
	lightning.setUniform("pointLights[2].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[2].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[2].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[2].linear", 0.7f);
	lightning.setUniform("pointLights[2].quadratic", 1.8f);


	lightning.setUniform("pointLights[3].position", pointLightsPositions[3]);
	lightning.setUniform("pointLights[3].constant", 1.0f);
	lightning.setUniform("pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[3].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[3].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[3].linear", 0.7f);
	lightning.setUniform("pointLights[3].quadratic", 1.8f);

	//compound lights
	lightning.setUniform("pointLights[4].position", pointLightsPositions[4]);
	lightning.setUniform("pointLights[4].constant", 1.0f);
	lightning.setUniform("pointLights[4].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[4].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[4].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[4].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[4].quadratic", COMPOUND_QUAD);

	lightning.setUniform("pointLights[5].position", pointLightsPositions[5]);
	lightning.setUniform("pointLights[5].constant", 1.0f);
	lightning.setUniform("pointLights[5].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[5].diffuse", COMPOUND_LIGHT); 
	lightning.setUniform("pointLights[5].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[5].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[5].quadratic", COMPOUND_QUAD);


	lightning.setUniform("pointLights[6].position", pointLightsPositions[6]);
	lightning.setUniform("pointLights[6].constant", 1.0f);
	lightning.setUniform("pointLights[6].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[6].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[6].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[6].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[6].quadratic", COMPOUND_QUAD);


	lightning.setUniform("pointLights[7].position", pointLightsPositions[7]);
	lightning.setUniform("pointLights[7].constant", 1.0f);
	lightning.setUniform("pointLights[7].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[7].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[7].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[7].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[7].quadratic", COMPOUND_QUAD);


	lightning.setUniform("pointLights[8].position", pointLightsPositions[8]);
	lightning.setUniform("pointLights[8].constant", 1.0f);
	lightning.setUniform("pointLights[8].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[8].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[8].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[8].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[8].quadratic", COMPOUND_QUAD);


	lightning.setUniform("pointLights[9].position", pointLightsPositions[9]);
	lightning.setUniform("pointLights[9].constant", 1.0f);
	lightning.setUniform("pointLights[9].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[9].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[9].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[9].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[9].quadratic", COMPOUND_QUAD);
	

	lightning.setUniform("pointLights[10].position", pointLightsPositions[10]);
	lightning.setUniform("pointLights[10].constant", 1.0f);
	lightning.setUniform("pointLights[10].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[10].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[10].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[10].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[10].quadratic", COMPOUND_QUAD);
	
	
	lightning.setUniform("pointLights[10].position", pointLightsPositions[10]);
	lightning.setUniform("pointLights[10].constant", 1.0f);
	lightning.setUniform("pointLights[10].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[10].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[10].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[10].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[10].quadratic", COMPOUND_QUAD);


	lightning.setUniform("pointLights[11].position", pointLightsPositions[11]);
	lightning.setUniform("pointLights[11].constant", 1.0f);
	lightning.setUniform("pointLights[11].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[11].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[11].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[11].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[11].quadratic", COMPOUND_QUAD);

	lightning.setUniform("pointLights[12].position", pointLightsPositions[12]);
	lightning.setUniform("pointLights[12].constant", 1.0f);
	lightning.setUniform("pointLights[12].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[12].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[12].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[12].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[12].quadratic", COMPOUND_QUAD);


	lightning.setUniform("pointLights[13].position", pointLightsPositions[13]);
	lightning.setUniform("pointLights[13].constant", 1.0f);
	lightning.setUniform("pointLights[13].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[13].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[13].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[13].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[13].quadratic", COMPOUND_QUAD);

	lightning.setUniform("pointLights[14].position", pointLightsPositions[14]);
	lightning.setUniform("pointLights[14].constant", 1.0f);
	lightning.setUniform("pointLights[14].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[14].diffuse", COMPOUND_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[14].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[14].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[14].quadratic", COMPOUND_QUAD);

	lightning.setUniform("pointLights[15].position", pointLightsPositions[15]);
	lightning.setUniform("pointLights[15].constant", 1.0f);
	lightning.setUniform("pointLights[15].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[15].diffuse", 0.3, 0.3, 0.3); // darken diffuse light a bit
	lightning.setUniform("pointLights[15].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[15].linear", COMPOUND_LINEAR);
	lightning.setUniform("pointLights[15].quadratic", COMPOUND_QUAD);

	//gajur lights
	lightning.setUniform("pointLights[16].position", pointLightsPositions[16]);
	lightning.setUniform("pointLights[16].constant", 1.0f);
	lightning.setUniform("pointLights[16].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[16].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[16].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[16].linear", GAJUR_LINEAR);
	lightning.setUniform("pointLights[16].quadratic", GAJUR_QUAD);

	lightning.setUniform("pointLights[17].position", pointLightsPositions[17]);
	lightning.setUniform("pointLights[17].constant", 1.0f);
	lightning.setUniform("pointLights[17].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[17].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[17].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[17].linear", GAJUR_LINEAR);
	lightning.setUniform("pointLights[17].quadratic", GAJUR_QUAD);


	lightning.setUniform("pointLights[18].position", pointLightsPositions[18]);
	lightning.setUniform("pointLights[18].constant", 1.0f);
	lightning.setUniform("pointLights[18].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[18].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[18].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[18].linear", GAJUR_LINEAR);
	lightning.setUniform("pointLights[18].quadratic", GAJUR_QUAD);


	lightning.setUniform("pointLights[19].position", pointLightsPositions[19]);
	lightning.setUniform("pointLights[19].constant", 1.0f);
	lightning.setUniform("pointLights[19].ambient", 0.2f, 0.2f, 0.2f);
	lightning.setUniform("pointLights[19].diffuse", GAJUR_LIGHT); // darken diffuse light a bit
	lightning.setUniform("pointLights[19].specular", SPECULAR, SPECULAR,SPECULAR);
	lightning.setUniform("pointLights[19].linear", GAJUR_LINEAR);
	lightning.setUniform("pointLights[19].quadratic", GAJUR_QUAD);

}


int main() {
	Renderer renderer;

	glfwInit();

	const int width = 800, height = 800;
	Renderer::width = width;
	Renderer::height = height;
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
	context.logMode = false;
	context.sensitivity= 4.0;

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

	std::string str((char*)vendor);
	if (str.find("ATI") != std::string::npos) {
		std::cout << "Use other card unless you want to get bsod";
		exit(-1);
	}

	//For blending, i.e. for textures with RGBA values
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//shader
	Shader lightShader("../resources/shaders/light.glsl");
	Shader lampShader("../resources/shaders/lamp.glsl");
	Shader lightning("../resources/shaders/lightmap.glsl");
	Shader waterShader("../resources/shaders/water.glsl");

	//dudvmap texture
	Texture dudv("../resources/textures/dudv.png");


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
	projection = MathLib::perspective(to_radians(fov), (float)width / (float)height, 0.00000001f, 100.0f);

	//reflection matrix
	MathLib::mat4 reflect(1.0f);


	//skybox shader uniform
	skyBoxShader.bind();
	
	//camera
	MathLib::vec3 cameraPos = MathLib::vec3(0.0f, 60.0f,100.0f);
	// MathLib::vec3 cameraPos = MathLib::vec3(54.11,3.92044,-55.4537);
	MathLib::vec3 cameraFront = MathLib::vec3(0.0f, 0.0f, -1.0f);
	MathLib::vec3 cameraUp = MathLib::vec3(0.0f, 1.0f, 0.0f);
	Camera camera(cameraPos, cameraFront, cameraUp);
	renderer.camera = camera;

	// glEnable(GL_DEPTH_TEST);

	context.renderer = &renderer;
	context.fov = fov;
	glfwSetWindowUserPointer(window, &context);


	//model
	Shader modelShader("../resources/shaders/model.glsl");


	// Model temple("../resources/models/Raanipokhari_stone base.obj");
	std::cout << "Started\n";
	Model temple("../resources/models/RaanipokhariWithtree.obj");
	std::cout << "Finished\n";
	Model templeOnly("../resources/models/mandir.obj");
	Model stone("../resources/models/stone.obj");
	Model water("../resources/models/water.obj");
	Model sun("../resources/models/sun.obj");


	WaterFrameBuffer waterFBO;
	waterFBO.initReflectionFrameBuffer();
	// waterFBO.initRefractionFrameBuffer();
	waterFBO.unbind();

	bool renderToTextureFlag;
	float moveFactor = 0.0f;


	//zbuffer code  // Manual depth buffering
  auto zrender = ZRender(width,height);
  auto zprogram = glCreateProgram();
  auto zvertex  = CompileAndLogShader("../resources/shaders/zvertex.glsl",GL_VERTEX_SHADER);
  glAttachShader(zprogram,zvertex);
  zrender.attachDepthFragment(zprogram);

	// glViewport(0, 0, width, height);
	while (!glfwWindowShouldClose(window)) {
		if(context.logMode){
			// std::cout << "Camera Position:" << renderer.camera.cameraPosition << std::endl;
			// std::cout << "Camera Direction:" << renderer.camera.cameraFront << std::endl;
		}

		//renderer.clear(0.6f, 0.8f, 0.8f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    zrender.clearBG(0.0f, 0.0f, 0.0f, 1.0f);
	    zrender.clearDepth();

		glDepthMask(GL_FALSE);
		//skybox
		skyBoxShader.bind();
		MathLib::mat4 view = MathLib::mat4(MathLib::mat3(renderer.camera.GetLookAtMatrix()));
		skyBoxShader.setUniform("view", view);
		skyBoxShader.setUniform("projection", projection);
		skyBox.bind();
		renderer.draw(skyBoxVA, skyBoxShader, 36);
		glDepthMask(GL_TRUE);
		skyBoxShader.unbind();
		
		
		projection = MathLib::perspective(to_radians(context.fov), aspectRatio, 0.1f, 1000.0f);
		MathLib::mat4 trans = MathLib::mat4(1.0f);
		MathLib::mat4 model = MathLib::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		view = renderer.camera.GetLookAtMatrix();

		zrender.clearDepth();

		glUseProgram(zprogram);
 	   update_uniform_matrix_4f("model", zprogram, model.value_ptr());
 	   update_uniform_matrix_4f("projection", zprogram, projection.value_ptr());//projection.value_ptr());
 	   update_uniform_matrix_4f("view", zprogram, view.value_ptr());

		zrender.clearDepth();

		//setting for models
		float angle = 0.0f;
		float timeValue = (float)glfwGetTime();




		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterFBO.reflectionFrameBuffer.renderedTexture);
		waterShader.bind();
		model = MathLib::mat4(1.0f);
		model = MathLib::rotate(model, to_radians(angle), MathLib::vec3(0.5f, -0.5f, 0.5f));
		angle = 0.0f;
		trans = MathLib::mat4(1.0f);
		trans = MathLib::translate(trans, MathLib::vec3(0.0f, 0.8f, 0.0f));
		// trans = MathLib::scale(trans, MathLib::vec3(0.4f, 0.4f, 0.4f));
		//wave speed calculation
		float waveSpeed = 0.04f;
		moveFactor += waveSpeed * timeValue; 
		waterShader.setUniform("model", model * trans);
		waterShader.setUniform("projection", projection);
		waterShader.setUniform("view", view);
		waterShader.setUniform("material.shininess", 128.0f);
		waterShader.setUniform("viewPos", camera.cameraPosition);
		waterShader.setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
		waterShader.setUniform("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		waterShader.setUniform("light.specular", SPECULAR, SPECULAR,SPECULAR);
		waterShader.setUniform("light.position", sunpos);
		waterShader.setUniform("isNightMode", context.isNightMode);
		waterShader.setUniform("moveFactor",  timeValue * waveSpeed);

		glActiveTexture(GL_TEXTURE0 + 1);
		waterShader.setUniform("dudv", static_cast<int>(1));
		dudv.bind(1);
		glActiveTexture(GL_TEXTURE0 + 2);
		waterShader.setUniform("reflectionTexture", static_cast<int>(2));
		glBindTexture(GL_TEXTURE_2D, waterFBO.reflectionFrameBuffer.renderedTexture);
		setLightPosition(waterShader);
		glCheckError(water.render(waterShader,zprogram, zrender, true));
		waterShader.unbind();



		lightning.bind();
		model = MathLib::mat4(1.0f);
		model = MathLib::rotate(model, to_radians(angle), MathLib::vec3(0.5f, -0.5f, 0.5f));
		angle = 0.0f;
		trans = MathLib::mat4(1.0f);
		trans = MathLib::translate(trans, MathLib::vec3(0.0f, -2.5f, -2.0f));
		
	
		float pt = int(timeValue) % 45*4;//converted 45 sec tie value to 180 degree to be use in light direction
		model = MathLib::translate(model, MathLib::vec3(0.0f, 3.0f, 0.0f));
		lightning.setUniform("lightMatrix", model);
		lightning.setUniform("projection", projection);
		lightning.setUniform("view", view);
		lightning.setUniform("material.shininess", 64.0f);
		lightning.setUniform("viewPos", camera.cameraPosition);

		lightning.setUniform("isNightMode", (int)context.isNightMode);		

		setLightPosition(lightning);
		//sun light
		lightning.setUniform("light.ambient", 0.2f, 0.2f, 0.2f);
		lightning.setUniform("light.diffuse", 0.3f, 0.3f, 0.3f); // darken diffuse light a bit
		lightning.setUniform("light.specular", SPECULAR, SPECULAR,SPECULAR);
		lightning.setUniform("light.position", sunpos);

		//write into framebuffer
		// waterFBO.bindReflectionFrameBuffer();
		// // reflect = MathLib::scale(reflect, MathLib::vec3(0.5, 0.5f, 0.0));
		// reflect[1][1] = -1.0f;
		// reflect = reflect * trans;
		// lightning.setUniform("model", model * reflect);
		// renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// lightning.setUniform("plane", 0, 1, 0, -15);
		// temple.render(lightning, true);
		// waterFBO.unbind();


		lightning.setUniform("model", model * trans);
		lightning.setUniform("plane", 0, -1, 0, 30);
		lightning.setUniform("isReflection", static_cast<int>(0));

		//translate
		temple.render(lightning, zprogram, zrender, true);
		reflect = MathLib::mat4(1.0f);
		// reflect[1][1] = -1.0f;
		reflect = reflect*trans;
		reflect = MathLib::translate(reflect, MathLib::vec3(0.0f, 5.3f, 2.0f));
		reflect[1][1] *= -1.0f;

		//view matrix for relflection so that the relection is fix
		MathLib::mat4 reflectionView = MathLib::lookAt(MathLib::vec3(0, 10, 70), MathLib::vec3(-0.055, -0.011, -1.0), MathLib::vec3(0.0f, 1.0f, 0.0f)); 

		lightning.setUniform("model", model * reflect);
		lightning.setUniform("isReflection", static_cast<int>(1));
		// templeOnly.render(lightning, zprogram, zrender, true);



		//stone render after reflection
		trans = MathLib::translate(trans, MathLib::vec3(0.0f, 1.5f, 0.0));
		lightning.setUniform("model", model * trans);
		lightning.setUniform("isReflection", static_cast<int>(0));
		// stone.render(lightning, zprogram, zrender, true);
		lightning.unbind();




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
		//render the sun only in day mode
		if(!context.isNightMode)
			glCheckError(sun.render(lampShader,zprogram, zrender,  true));
		lampShader.unbind();

		zrender.draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
		renderer.processKeyboardInput(window);

	}

}