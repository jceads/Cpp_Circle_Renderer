#pragma once
#include "glm/vec3.hpp"

namespace OpenGL
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
    };
}
