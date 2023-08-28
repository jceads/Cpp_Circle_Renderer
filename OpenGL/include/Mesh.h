#pragma once
#include "Shader.h"
#include <vector>

#include "Mesh/BufferObjects.h"
#include "Mesh/Texture.h"
#include "Mesh/Vertex.h"


namespace OpenGL
{
    class Mesh
    {
    public:
        ~Mesh();
        std::vector<Vertex>   vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture>  textures;

        Mesh(const std::vector<Vertex>&  vertices, const std::vector<unsigned int>& indices,
             const std::vector<Texture>& textures);
        void Draw(const Shader& shader);

    private:
        void          SetupMesh();
        void          GenerateBuffer(GLuint* buff);
        void          GenerateVAO(GLuint* vao);
        uint32_t      m_IndexCount  = 0;
        uint32_t      m_VertexCount = 0;
        BufferObjects buffer_objects;
    };
}
