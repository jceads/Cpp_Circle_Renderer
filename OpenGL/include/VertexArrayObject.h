#pragma once
#include <vector>

#include "../src/Model/Vertex.h"

namespace OpenGL
{
    class VertexArrayObject
    {
    public:
        VertexArrayObject();
        void        Build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        void        Activate() const;
        static void DeActivate();

    private:
        unsigned int m_VaoId;
        unsigned int m_VboId;
        unsigned int m_VibId;
    };
}
