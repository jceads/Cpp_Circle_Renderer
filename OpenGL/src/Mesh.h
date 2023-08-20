#pragma once
#include "VertexArrayObject.h"
#include "MeshManager.h"


namespace OpenGL
{
    class Mesh
    {
    public:
        void Draw() const;

    private:
        VertexArrayObject* m_Vao         = nullptr;
        uint32_t           m_IndexCount  = 0;
        uint32_t           m_VertexCount = 0;
        friend MeshManager;
    };
}
