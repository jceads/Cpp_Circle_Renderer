#pragma once
#include "Shader.h"
#include "VertexArrayObject.h"


namespace OpenGL
{
    class Mesh
    {
    public:
        ~Mesh();
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int              VAO;

        Mesh(const std::vector<Vertex>&  vertices, const std::vector<unsigned int>& indices,
             const std::vector<Texture>& textures);
        void Draw(Shader& shader);

    private:
        void     SetupMesh();
        uint32_t m_IndexCount  = 0;
        uint32_t m_VertexCount = 0;
    };
}
