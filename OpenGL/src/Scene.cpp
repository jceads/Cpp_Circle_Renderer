#include "Scene.h"

#include <iostream>

#include "Model.h"
#include "assimp/postprocess.h"
#include "imgui/imgui.h"


OpenGL::Scene::Scene()
{
    m_Camera.Position = glm::vec3{1.5f, 1.0f, 5.0f};
    AddSampleMesh();
    AddLightSource();
}

OpenGL::Scene::~Scene()
{
    Scene::Dispose();
}


void OpenGL::Scene::Draw()
{
    // float* cam_pos[3] = {&m_camera.Position.x, &m_camera.Position.y, &m_camera.Position.z};
    ImGui::SeparatorText("Camera");
    {
        ImGui::DragFloat("x", &m_Camera.Position.x, 0.01f);
        ImGui::DragFloat("y", &m_Camera.Position.y, 0.01f);
        ImGui::DragFloat("z", &m_Camera.Position.z, 0.01f);
        for (int i = 0; i < m_SceneMeshes.size(); i++)
            m_SceneMeshes[i]->Draw(&m_Camera);
    }
}

void OpenGL::Scene::Dispose()
{
    IDisposable::Dispose();
    for (const auto& mesh : m_SceneMeshes)
    {
        delete mesh;
    }
}

void OpenGL::Scene::AddSampleMesh()
{
    const std::string path{"./data/ico_sphere.fbx"};
    auto const&       model = new Model(path);
    model->InitShader("./data/shaders/vertex.glsl", "./data/shaders/fragment.glsl");
    model->addAttrib3("objectColor",
                      glm::vec3{model->color.r, model->color.g, model->color.b});
    model->addAttrib3("lightColor", glm::vec3{1.0f, 1.0f, 1.0f});
    model->addAttrib3("lightPost", glm::vec3{1.2f, 1.0f, 2.0f});
    model->addAttrib3("viewPos", m_Camera.Position);

    m_SceneMeshes.push_back(model);
}

void OpenGL::Scene::AddLightSource()
{
    const std::string path{"./data/little_cube.fbx"};
    auto const&       model = new Model(path);
    model->InitShader("./data/shaders/VLight.glsl", "./data/shaders/FLight.glsl");
    model->transform = OpenGL::Transform{
        glm::vec3{1.0f, 2.0f, 1.0f}, glm::vec3{1.0f}, glm::vec3{1.0f}
    };
    model->color = Color{0.5f};
    m_SceneMeshes.push_back(model);
}
