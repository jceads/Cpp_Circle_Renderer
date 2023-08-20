#pragma once
#include "glm/vec3.hpp"

class Camera
{
public:
    Camera(float     fov       = 30.0f,
           float     nearPlane = 0.1f,
           float     farPlane  = 1000.0f,
           glm::vec3 position  = glm::vec3{0.0f, 0.0f, 0.0f},
           glm::vec3 lookAt    = glm::vec3{0.0f, 0.0f, 1.0f});
    float fov,
          nearPlane
          , farPlane;

    void ChangePosition(glm::vec3 newPosition);


    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;
};
