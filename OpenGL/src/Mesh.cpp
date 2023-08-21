#include "Mesh.h"

#include <iostream>
#include <stdexcept>

#include "GL/glew.h"


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


OpenGL::Mesh::~Mesh()
= default;

OpenGL::Mesh::Mesh(const std::vector<Vertex>&  vertices, const std::vector<unsigned>& indices,
                   const std::vector<Texture>& textures)
    : vertices(vertices), indices(indices), textures(textures)
{
    SetupMesh();
}

void OpenGL::Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        GlCall(glActiveTexture(GL_TEXTURE0 + i));
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);
        GlCall(glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i));
        GlCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }
    GlCall(glBindVertexArray(VAO));
    GlCall(glDrawElements(GL_TRIANGLES, indices.size(),GL_UNSIGNED_INT, 0));
    GlCall(glBindVertexArray(0));
    GlCall(glActiveTexture(GL_TEXTURE0));
}

void OpenGL::Mesh::SetupMesh()
{
    // m_Vao.Build(this->vertices, this->indices);
    unsigned int VBO, EBO;
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
    m_VertexCount = vertices.size();
    m_IndexCount  = indices.size();
}
