#pragma once
#include <vector>
#include "Vertex.h"


namespace OpenGL
{
    class VertexArrayObject
    {
    public:
        VertexArrayObject();
        ~VertexArrayObject();
        unsigned int VAO;
        void         Build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        int          GetIndexCount() const;
        int          GetVertexCount() const;

    private:
        unsigned int VBO;
        unsigned int EBO;
        long int     m_IndexCount;
        long int     m_vertexCount;
    };
}
