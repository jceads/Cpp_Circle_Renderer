#pragma once
#include <map>
#include <string>

#include "VertexArrayObject.h"

namespace OpenGL
{
    class Mesh;

    class MeshManager
    {
    public:
        static MeshManager* GetInstance();
        ~MeshManager();
        Mesh* createCube();

    private:
        MeshManager() = default;
        static MeshManager*                       m_Instance;
        std::map<std::string, VertexArrayObject*> m_VaoMap;
    };
}
