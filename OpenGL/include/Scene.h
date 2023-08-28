#pragma once
#include "Camera.h"
#include "Model.h"
#include "Utils/IDisposable.h"
#include "InputListener.h"

namespace OpenGL
{
    class Scene final : IDisposable, protected InputListener
    {
    public:
        Scene();
        ~Scene() override;
        void   Draw();
        void   Notify(int key, int action) override;
        void   Dispose() override;
        Camera get_camera() const { return m_Camera; }

    private:
        std::vector<Model*> m_SceneModels;
        void                AddSampleMesh(const std::string& path);
        void                AddLightSource(const std::string& path);
        Camera              m_Camera;
        float               ambientLight = 0.1f;
    };
}
