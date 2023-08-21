#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"


namespace OpenGL
{
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };

    class Camera
    {
    public:
        Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 5.0f),
               glm::vec3 up  = glm::vec3(0.0f, 1.0f, 0.0f),
               float     yaw = -90.0f, float pitch = 0.0f);
        glm::mat4 GetViewMatrix();
        void      ProcessKeyboard(CameraMovement direction, float deltaTime);
        void      ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void      ProcessMouseScroll(float yOffset);

        float fov,
              nearPlane,
              farPlane;
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw,
              Pitch;

        float MovementSpeed,
              MouseSensitivity,
              Zoom;

        void ChangePosition(glm::vec3 newPosition);

    private:
        void updateCameraVectors();
    };
}
