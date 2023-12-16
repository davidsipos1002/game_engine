#include <Camera.hpp>

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = -this->cameraTarget;
        this->cameraRightDirection = glm::cross(cameraUp, cameraTarget); 
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, this->cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction) {
            case MOVE_FORWARD:
                cameraPosition += cameraFrontDirection * speed;
                break;
            case MOVE_BACKWARD:
                cameraPosition -= cameraFrontDirection * speed;
                break;
            case MOVE_LEFT:
                cameraPosition -= cameraRightDirection * speed;
                break;
            case MOVE_RIGHT:
                cameraPosition += cameraRightDirection * speed;
                break;
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        glm::vec3 dir(
            cos(yaw) * sin(pitch),
            sin(yaw),
            cos(yaw) * cos(pitch)
        );
        glm::vec3 right = glm::vec3(
            sin(pitch - 3.14f/2.0f),
            0,
            cos(pitch - 3.14f/2.0f)
        );
        glm::vec3 up = glm::cross( right, dir );

        this->cameraTarget = this->cameraPosition + dir;
        this->cameraUpDirection = up;
        this->cameraRightDirection = right;
        this->cameraFrontDirection = -this->cameraTarget;
    }
}
