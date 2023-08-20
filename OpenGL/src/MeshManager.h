#pragma once
#include <map>
#include <string>

#include "VAOManager.h"
#include "VertexArrayObject.h"

namespace OpenGL
{
    class Mesh;

    class MeshManager
    {
    public:
        ~MeshManager();
        MeshManager(VAOManager* vao_manager);
        Mesh* createCube() const;

    private:
        VAOManager* m_VaoManager;
    };
}
