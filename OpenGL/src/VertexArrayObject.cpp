#include "VertexArrayObject.h"

#include "GL/glew.h"
#include "glm/glm.hpp"

OpenGL::VertexArrayObject::VertexArrayObject()
{
    m_VaoId = 0;
    m_VboId = 0;
    m_VibId = 0;
}

OpenGL::VertexArrayObject::~VertexArrayObject()
{
    if (m_VaoId)
        glDeleteVertexArrays(1, &m_VaoId);
    if (m_VibId)
        glDeleteBuffers(1, &m_VibId);
    if (m_VboId)
        glDeleteBuffers(1, &m_VboId);
}

void OpenGL::VertexArrayObject::Build(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices)
{
    glGenVertexArrays(1, &m_VaoId);

    glGenBuffers(1, &m_VboId);

    glGenBuffers(1, &m_VibId);

    glBindVertexArray(m_VaoId);


    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

    glBufferData(GL_ARRAY_BUFFER, sizeof(OpenGL::Vertex) * vertices.size(), vertices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 9 * sizeof(float), static_cast<void*>(0));

    glVertexAttribPointer(1, 4,GL_FLOAT,GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3));

    glVertexAttribPointer(2, 2,GL_FLOAT,GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 7));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VibId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(),GL_STATIC_DRAW);

    m_AttributeList.push_back(0);
    m_AttributeList.push_back(1);
    m_AttributeList.push_back(2);
    m_vertexCount = vertices.size();
    m_IndexCount  = indices.size();
}

void OpenGL::VertexArrayObject::Activate() const
{
    glBindVertexArray(m_VaoId);
    for (auto& next : m_AttributeList)
    {
        glEnableVertexAttribArray(next);
    }
}

void OpenGL::VertexArrayObject::DeActivate()
{
    glBindVertexArray(0);
}

int OpenGL::VertexArrayObject::GetIndexCount() const
{
    return m_IndexCount;
}

int OpenGL::VertexArrayObject::GetVertexCount() const
{
    return m_vertexCount;
}
