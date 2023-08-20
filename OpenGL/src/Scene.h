#pragma once
#include "MeshManager.h"
#include "SceneObject.h"
#include "Texture.h"

namespace OpenGL
{
    class Scene
    {
    public:
        Scene();
        ~Scene() = default;
        void Draw();

    private:
        std::vector<SceneObject> m_sceneObjects;
        void                     AddCube();
    };

    // class Scene
    // {
    // public:
    //     Scene();
    //     ~Scene();
    //     MeshManager* GetMeshManager() const;
    //     Texture*     GetTextureManager() const;
    //     VAOManager*  GetVaoManager() const;
    //
    // private:
    //     MeshManager* m_MeshManager;
    //     Texture*     m_TextureManager;
    //     VAOManager*  m_VaoManager;
    // };
}
