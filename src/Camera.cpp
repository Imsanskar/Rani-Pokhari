#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float _yaw, float _pitch)
	  :	cameraPosition(position),
		cameraFront(front),
		cameraUp(up),
        yaw(_yaw),
        pitch(_pitch)
{
    //cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    sensitivity = 0.2f;
}

glm::mat4 Camera::GetLookAtMatrix() const{
	return glm::lookAt(cameraPosition, cameraPosition + glm::normalize(cameraFront), cameraUp);
}

void Camera::processKeyboardEvent(CameraDirection direction, float deltaTime){
    float cameraSpeed = 0.9f * deltaTime;

    if (direction == CameraDirection::FORWARD) {
        cameraPosition += cameraSpeed * glm::normalize(cameraFront);
    }
    if (direction == CameraDirection::LEFT) {
        cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (direction == CameraDirection::REVERSE) {
        cameraPosition -= cameraSpeed * glm::normalize(cameraFront);
    }
    if (direction == CameraDirection::RIGHT) {
        cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
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
    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}

void Camera::updateVectors(){
    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
    //cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    //cameraUp = glm::normalize(glm::cross(cameraRight, cameraUp));
}
