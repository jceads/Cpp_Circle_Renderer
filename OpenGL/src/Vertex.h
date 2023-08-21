#pragma once
#include "glm/glm.hpp"
#include <string>

namespace OpenGL
{
#define MAX_BONE_INFLUENCE 4

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec2 Tangent;
        glm::vec2 BiTangent;

        int   m_BoneIDs[MAX_BONE_INFLUENCE];
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Texture
    {
        unsigned int id;
        std::string  type;
        std::string  path;
    };
}
