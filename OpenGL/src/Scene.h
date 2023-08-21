#pragma once
#include "Camera.h"
#include "Model.h"

namespace OpenGL
{
    class Scene
    {
    public:
        Scene();
        ~Scene() = default;
        void Draw();

    private:
        std::vector<Model*> m_SceneMeshes;
        Camera              m_camera;
        void                AddCube();
    };
}
