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
        void Build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        void Activate() const;
        void DeActivate();
        int  GetIndexCount() const;
        int  getVertexCount() const;

    private:
        unsigned int m_VaoId;
        unsigned int m_VboId;
        unsigned int m_VibId;
        uint32_t     m_IndexCount;
        uint32_t     m_vertexCount;

        std::vector<unsigned int> m_AttributeList;
    };
}
