#include <objects/Camera.hpp>
#include <iostream>

namespace gps
{
    Camera::Camera(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &cameraUp, Window *window)
    {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = glm::normalize(cameraTarget);
        this->cameraUpDirection = glm::normalize(cameraUp);
        this->cameraFrontDirection = this->cameraTarget;
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, this->cameraUpDirection));
        projectionMatrix = glm::perspective(glm::radians(45.0f),
                                      (float)window->getWindowDimensions().width / (float)window->getWindowDimensions().height,
                                      0.1f, 20.0f);
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, this->cameraUpDirection);
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
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, this->cameraUpDirection);
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
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, this->cameraUpDirection);
    }

    const glm::mat4 &Camera::getViewMatrix()
    {
        return viewMatrix;
    }
    
    const glm::mat4 &Camera::getProjectionMatrix()
    {
        return projectionMatrix;
    }
}
