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

    private:
        unsigned int              m_VaoId;
        unsigned int              m_VboId;
        unsigned int              m_VibId;
        std::vector<unsigned int> m_AttributeList;
    };
}
