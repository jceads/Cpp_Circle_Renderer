#pragma once
#include <glm/glm.hpp>

namespace OpenGL
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texture;
    };
}
