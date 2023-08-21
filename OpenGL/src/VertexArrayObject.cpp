#include "VertexArrayObject.h"

#include <iostream>

#include "GL/glew.h"
#include "glm/glm.hpp"


#ifndef GLGuard
#define GLGuard

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
    x;\
    ASSERT(GlLogCall(#x, __FILE__,__LINE__))

static void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* func, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Opengl Error] (" << error << ") " << func << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}
#endif


OpenGL::VertexArrayObject::VertexArrayObject()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
}

OpenGL::VertexArrayObject::~VertexArrayObject()
{
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (EBO)
        glDeleteBuffers(1, &EBO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
}

void OpenGL::VertexArrayObject::Build(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices)
{
    GlCall(glGenVertexArrays(1, &VAO))

    GlCall(glGenBuffers(1, &VBO));

    GlCall(glGenBuffers(1, &EBO));

    GlCall(glBindVertexArray(VAO));


    GlCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    GlCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(OpenGL::Vertex), vertices.data(),GL_STATIC_DRAW));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),GL_STATIC_DRAW));

    GlCall(glEnableVertexAttribArray(0));
    GlCall(glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, sizeof(Vertex), static_cast<void*>(0)));

    GlCall(glEnableVertexAttribArray(1));
    GlCall(glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

    GlCall(glEnableVertexAttribArray(2));
    GlCall(glVertexAttribPointer(2, 2,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

    GlCall(glEnableVertexAttribArray(3));
    GlCall(glVertexAttribPointer(3, 3,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));

    GlCall(glEnableVertexAttribArray(4));
    GlCall(glVertexAttribPointer(4, 3,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent)));
    GlCall(glBindVertexArray(0));
    m_vertexCount = vertices.size();
    m_IndexCount  = indices.size();
}

int OpenGL::VertexArrayObject::GetIndexCount() const
{
    return m_IndexCount;
}

int OpenGL::VertexArrayObject::GetVertexCount() const
{
    return m_vertexCount;
}
