#pragma once
#include "MeshManager.h"
#include "TextureManager.h"

namespace OpenGL
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        MeshManager*    GetMeshManager() const;
        TextureManager* GetTextureManager() const;
        VAOManager*     GetVaoManager() const;

    private:
        MeshManager*    m_MeshManager;
        TextureManager* m_TextureManager;
        VAOManager*     m_VaoManager;
    };
}
