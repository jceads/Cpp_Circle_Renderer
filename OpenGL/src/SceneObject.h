#pragma once
#include <vector>

#include "Shader.h"

class SceneObject
{
public:
private:
    OpenGL::Shader m_shader;
    std::vector<glm::vec3> m_vertices;
    std::vector<unsigned int> m_indices;
    
};
