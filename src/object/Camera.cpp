#include <object/Camera.hpp>
#include <iostream>

namespace gps
{

    // Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp)
    {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = glm::normalize(cameraTarget);
        this->cameraUpDirection = glm::normalize(cameraUp);
        this->cameraFrontDirection = this->cameraTarget;
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, this->cameraUpDirection));
    }

    // return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(cameraPosition, cameraPosition + cameraTarget, this->cameraUpDirection);
    }

    // update the camera internal parameters following a camera move event
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

    // update the camera internal parameters following a camera rotate event
    // yaw - camera rotation around the y axis
    // pitch - camera rotation around the x axis
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
