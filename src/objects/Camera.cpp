#include <objects/Camera.hpp>
#include <iostream>

namespace gps
{
    Camera::Camera(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &cameraUp)
    {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = glm::normalize(cameraTarget);
        this->cameraUpDirection = glm::normalize(cameraUp);
        this->cameraFrontDirection = this->cameraTarget;
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, this->cameraUpDirection));
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(cameraPosition, cameraPosition + cameraTarget, this->cameraUpDirection);
    }

    void Camera::move(MOVE_DIRECTION direction, float speed)
    {
        switch (direction)
        {
        case MOVE_FORWARD:
            cameraPosition += cameraFrontDirection * speed;
            break;
        case MOVE_BACKWARD:
            cameraPosition -= cameraFrontDirection * speed;
            break;
        case MOVE_LEFT:
            cameraPosition -= cameraRightDirection * speed;
            cameraTarget -= cameraRightDirection * speed;
            break;
        case MOVE_RIGHT:
            cameraPosition += cameraRightDirection * speed;
            cameraTarget += cameraRightDirection * speed;
            break;
        }
    }

    void Camera::rotate(float pitch, float yaw)
    {
        glm::vec3 dir(
            cos(pitch) * cos(yaw),
            sin(yaw),
            -sin(pitch) * cos(yaw));

        this->cameraTarget = glm::normalize(dir);
        this->cameraRightDirection = glm::normalize(glm::cross(dir, cameraUpDirection));
        this->cameraFrontDirection = this->cameraTarget;
    }
}
