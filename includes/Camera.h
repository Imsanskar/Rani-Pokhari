#pragma once
#include <Math/matrix.h>


enum class CameraDirection {
	FORWARD,
	REVERSE,
	LEFT,
	RIGHT
};


struct Camera {
	MathLib::vec3 cameraPosition, cameraFront, cameraUp, cameraRight;

	float sensitivity;

	float yaw, pitch;

	Camera(MathLib::vec3 position = MathLib::vec3(0.0f, 0.0f, 0.0f), MathLib::vec3 front = MathLib::vec3(0.0f, 0.0f, -1.0f), MathLib::vec3 up = MathLib::vec3(0.0f, 1.0f, 0.0f), float _yaw = -90.0f, float _pitch = 0.0f);

	MathLib::mat4 GetLookAtMatrix() const;

	void processKeyboardEvent(CameraDirection direction, float deltaTime);

	void processMouseEvent(float xOffset, float yOffset, bool constrainScreen = true);

	void updateVectors();
};