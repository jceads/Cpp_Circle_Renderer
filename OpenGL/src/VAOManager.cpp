#include "VAOManager.h"

void OpenGL::VAOManager::AddNewObject(std::string name, VertexArrayObject* vao)
{
    if (!m_VaoMap.count(name))
    {
        m_VaoMap[name] = vao;
    }
}

OpenGL::VertexArrayObject* OpenGL::VAOManager::GetVao(std::string name)
{
    if (m_VaoMap.count(name))
    {
        return m_VaoMap[name];
    }
    return nullptr;
}
