#pragma once
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

namespace OpenGL
{
    class SceneObject
    {
    public:
        SceneObject();
        ~SceneObject();
        void Draw();

    private:
        Shader*                   m_shader;
        Mesh*                     m_mesh;
        Texture*                  m_texture;
        std::vector<glm::vec3>    m_vertices;
        std::vector<unsigned int> m_indices;
    };
}
