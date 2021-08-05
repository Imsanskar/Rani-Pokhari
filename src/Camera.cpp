#include "Camera.h"

Camera::Camera(MathLib::vec3 position, MathLib::vec3 front, MathLib::vec3 up, float _yaw, float _pitch)
	  :	cameraPosition(position.x, position.y, position.z),
		cameraFront(front.x, front.y, front.z),
		cameraUp(up.x, up.y, up.z),
        yaw(_yaw),
        pitch(_pitch)
{
    //cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    sensitivity = 0.2f;
}

MathLib::mat4 Camera::GetLookAtMatrix() const{
	return MathLib::lookAt(cameraPosition, cameraPosition + MathLib::normalize(cameraFront), cameraUp);
}

void Camera::processKeyboardEvent(CameraDirection direction, float deltaTime){
    float cameraSpeed = sensitivity * deltaTime;

    if (direction == CameraDirection::FORWARD) {
        cameraPosition += cameraSpeed * cameraFront.unitVector();
    }
    if (direction == CameraDirection::LEFT) {
        cameraPosition -= cameraSpeed * MathLib::normalize(MathLib::cross(cameraFront, cameraUp));
    }
    if (direction == CameraDirection::REVERSE) {
        cameraPosition -= cameraSpeed * MathLib::normalize(cameraFront);
    }
    if (direction == CameraDirection::RIGHT) {
        cameraPosition += cameraSpeed * MathLib::normalize(MathLib::cross(cameraFront, cameraUp));
    }
}

void Camera::processMouseEvent(float xOffset, float yOffset, bool constrainScreen){
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrainScreen)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    MathLib::vec3 front;

    front.x = cos(to_radians(yaw)) * cos(to_radians(pitch));
    front.y = sin(to_radians(pitch));
    front.z = sin(to_radians(yaw)) * cos(to_radians(pitch));

	cameraFront = MathLib::normalize(front);
}