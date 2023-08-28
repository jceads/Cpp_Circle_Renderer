#include "Camera.h"

#include "WindowManager.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"

OpenGL::Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    : Position(pos),
      Front(glm::vec3(0.0f, 0.0f, -1.0f)), Up(up), fov(30),
      Yaw(yaw), Pitch(pitch), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(45.0f)
{
}

glm::mat4 OpenGL::Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void OpenGL::Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == CameraMovement::FORWARD)
        Position += Front * velocity;
    if (direction == CameraMovement::BACKWARD)
        Position -= Front * velocity;
    if (direction == CameraMovement::LEFT)
        Position -= Right * velocity;
    if (direction == CameraMovement::RIGHT)
        Position += Right * velocity;
}

void OpenGL::Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void OpenGL::Camera::ProcessMouseScroll(float yOffset)
{
    Zoom -= (float)yOffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void OpenGL::Camera::ChangePosition(glm::vec3 newPosition)
{
    Position = newPosition;
}

void OpenGL::Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front   = glm::normalize(front);
    //  re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}
