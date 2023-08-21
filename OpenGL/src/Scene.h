#pragma once
#include "Camera.h"
#include "Model.h"
#include "Utils/IDisposable.h"

namespace OpenGL
{
    class Scene : IDisposable
    {
    public:
        Scene();
        ~Scene() override;
        void Draw();

        void Dispose() override;

    private:
        std::vector<Model*> m_SceneMeshes;
        Camera              m_Camera;
        void                AddSampleMesh();
        void                AddLightSource();
    };
}
