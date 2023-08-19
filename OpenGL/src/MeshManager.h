#pragma once

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
        static MeshManager* m_Instance;
    };
}
