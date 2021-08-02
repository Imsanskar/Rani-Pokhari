#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float _yaw, float _pitch)
	  :	cameraPosition(position.x, position.y, position.z),
		cameraFront(front.x, front.y, front.z),
		cameraUp(up.x, up.y, up.z),
        yaw(_yaw),
        pitch(_pitch)
{
    //cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    sensitivity = 0.2f;
}

glm::mat4 Camera::GetLookAtMatrix() const{
	glm::vec3 cameraPos = glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glm::vec3 cameraFrontPos = glm::vec3(cameraFront.x, cameraFront.y, cameraFront.z);
	glm::vec3 cameraUpPos = glm::vec3(cameraUp.x, cameraUp.y, cameraUp.z);
	return glm::lookAt(cameraPos, cameraPos + glm::normalize(cameraFrontPos), cameraUpPos);
}

void Camera::processKeyboardEvent(CameraDirection direction, float deltaTime){
    float cameraSpeed = 0.9f * deltaTime;

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
    MathLib::Vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = MathLib::normalize(front);
}

void Camera::updateVectors(){
    MathLib::Vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = MathLib::normalize(front);
}
