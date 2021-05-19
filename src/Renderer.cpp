#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>

void GlClearError(){
	while (glGetError() != GL_NO_ERROR);
}

void _GlGetError(const char *function, const char *file, int line) {
	while(GLenum errorCode = glGetError()){
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << "[Opengl Error] (" << error << "):\nFunction: " << function <<"\nFile: "<< file << "\nLine:" << line << std::endl;
	}
}

int Renderer::width = 1600;
int Renderer::height = 900;

float Renderer::previousX = 800;
float Renderer::previousY = 450;


void mouseCallback(GLFWwindow* window, double xPos, double yPos){
    UserContext *context = (UserContext *)glfwGetWindowUserPointer(window);
    
    Renderer *renderer = context->renderer;

    if (renderer->firstMouse)
    {
        Renderer::previousX = xPos;
        Renderer::previousY = yPos;
        context->renderer->firstMouse = false;
    }
    float xOffset = xPos - Renderer::previousX;
    float yOffset = Renderer::previousY - yPos;

    const float sensitivity = 0.1f;

    Renderer::previousX = xPos;
    Renderer::previousY = yPos;

    renderer->camera.processMouseEvent(xOffset, yOffset);

    glfwSetWindowUserPointer(window, context);
}

void scrollCallBack(GLFWwindow *window, double xoffset, double yoffset) {
    UserContext* context = (UserContext*)glfwGetWindowUserPointer(window);
    float fov = context->fov;
    
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 150.0f)
        fov = 150.0f;

    context->fov = fov;
    glfwSetWindowUserPointer(window, context);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    UserContext* context = (UserContext*)glfwGetWindowUserPointer(window);
    Renderer::width = width;
    Renderer::height = height;
}





/*------------------------------------- RENDERER METHODS -----------------------------------------*/

void Renderer::clear(unsigned int bits) const{
    glClear(bits);
}

void Renderer::clear(float r, float g, float b, float a, unsigned int bits) const{
    glClearColor(r, g, b, a);
    glClear(bits);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
    shader.bind();
    va.bind();
    ib.bind();

    glCheckError(glDrawElements(GL_TRIANGLES, ib.count, GL_UNSIGNED_INT, NULL));
}

void Renderer::draw(const VertexArray& va, const Shader& shader, unsigned int vertexCount) const{
    va.bind();
    shader.bind();

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Renderer::processKeyboardInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    float cameraSpeed = 1.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.cameraPosition += cameraSpeed * glm::normalize(camera.cameraFront);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.cameraPosition -= cameraSpeed * glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.cameraPosition -= cameraSpeed * glm::normalize(camera.cameraFront);
    }if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.cameraPosition += cameraSpeed * glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp));
    }
}
