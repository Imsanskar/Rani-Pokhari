#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum class CameraDirection {
	FORWARD,
	REVERSE,
	LEFT,
	RIGHT
};


struct Camera {
	glm::vec3 cameraPosition, cameraFront, cameraUp, cameraRight;

	float sensitivity;

	float yaw, pitch;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = -90.0f, float _pitch = 0.0f);

	glm::mat4 GetLookAtMatrix() const;

	void processKeyboardEvent(CameraDirection direction, float deltaTime);

	void processMouseEvent(float xOffset, float yOffset, bool constrainScreen = true);

	void updateVectors();
};