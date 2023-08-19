#pragma once
#include "VertexArrayObject.h"
#include "../MeshManager.h"


namespace OpenGL
{
    class Mesh
    {
    public:
        void Draw();

    private:
        VertexArrayObject* m_Vao        = nullptr;
        uint32_t           m_IndexCount = 0;
        friend MeshManager;
    };
}
