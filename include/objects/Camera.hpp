#pragma once

#include <engine/GeneralIncludes.hpp>
#include <engine/Window.hpp>

namespace gps
{

    enum MOVE_DIRECTION
    {
        MOVE_FORWARD,
        MOVE_BACKWARD,
        MOVE_RIGHT,
        MOVE_LEFT
    };

    class Camera
    {

    public:
        Camera(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &cameraUp, Window *window);
        const glm::mat4 &getViewMatrix();
        const glm::mat4 &getProjectionMatrix();
        void move(MOVE_DIRECTION direction, float speed);
        void rotate(float pitch, float yaw);

    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };
}
