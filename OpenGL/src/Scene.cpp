#include "Scene.h"

#include <iostream>

#include "Model.h"
#include "WindowManager.h"
#include "assimp/postprocess.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/rotate_vector.hpp"
#include "imgui/imgui.h"

OpenGL::Scene::Scene()
{
    m_Camera.Position  = glm::vec3{1.5f, 1.0f, 5.0f};
    m_Camera.nearPlane = 0.01f;
    m_Camera.farPlane  = 1000.0f;
    KeyInputManager::AddListener(this);
    //AddSampleMesh("./data/ico_sphere.fbx");
    AddSampleMesh("./data/torus.fbx");
    //AddLightSource("./data/little_cube.fbx");
}

OpenGL::Scene::~Scene()
{
    Scene::Dispose();
}


void OpenGL::Scene::Draw()
{
    // float* cam_pos[3] = {&m_camera.Position.x, &m_camera.Position.y, &m_camera.Position.z};
    ImGui::SeparatorText("World");
    {
        ImGui::DragFloat("Ambient Light", &ambientLight, 0.01f, 0.0f, 1.0f);
        ImGui::SeparatorText("Camera");
        {
            ImGui::DragFloat3("position", &m_Camera.Position.x, 0.01f);
            ImGui::DragFloat("fov", &m_Camera.fov, 0.1f);
            ImGui::DragFloat("Near Plane", &m_Camera.nearPlane, 0.1f, 0.1f, 10.0f);
            ImGui::DragFloat("far Plane", &m_Camera.farPlane, 0.1f, 10.0f);
        }
    }
    for (const auto& m_SceneModel : m_SceneModels)
        m_SceneModel->Draw(&m_Camera, &ambientLight);
}

void OpenGL::Scene::Dispose()
{
    IDisposable::Dispose();
    for (const auto& mesh : m_SceneModels)
    {
        delete mesh;
    }
}

void OpenGL::Scene::AddSampleMesh(const std::string& path)
{
    auto const& model         = new Model(path);
    model->transform.position = glm::vec3{2.0f, 0.f, 0.f};
    model->InitShader("./data/shaders/vertex.glsl", "./data/shaders/fragment.glsl");

    glm::vec3 objectColor = glm::vec3{model->color.r, model->color.g, model->color.b};
    model->AddAttribute3("objectColor", &objectColor);

    glm::vec3 lightColor = glm::vec3{model->color.r, model->color.g, model->color.b};
    model->AddAttribute3("lightColor", &lightColor);

    model->AddAttributeF("ambientStrength", ambientLight);

    glm::vec3 lightPos = glm::vec3{1.2f, 1.0f, 2.0f};
    model->AddAttribute3("lightPost", &lightPos);
    model->AddAttribute3("viewPos", &m_Camera.Position);

    m_SceneModels.push_back(model);
}

void OpenGL::Scene::AddLightSource(const std::string& path)
{
    auto const& model = new Model(path);
    model->InitShader("./data/shaders/VLight.glsl", "./data/shaders/FLight.glsl");
    model->transform = OpenGL::Transform{
        glm::vec3{1.0f, 2.0f, 1.0f}, glm::vec3{1.0f}, glm::vec3{1.0f}
    };
    model->color = Color{0.5f, 0.5f, 0.5f, 1.0f};
    m_SceneModels.push_back(model);
}

void OpenGL::Scene::Notify(int key, int action)
{
    InputListener::Notify(key, action);
    std::cout << "Pressed to: " << key << " Action is: " << action << "input listener is: Scene";
    if (key == GLFW_KEY_ESCAPE)
    {
        exit(0);
    }
    if (key == GLFW_KEY_A)
    {
        m_Camera.Position -= glm::vec3{.1f, 0.0f, 0.0f};
    }

    if (key == GLFW_KEY_D)
    {
        m_Camera.Position += glm::vec3{.1f, 0.0f, 0.0f};
    }
    if (key == GLFW_KEY_Q)
    {
        m_Camera.Position += glm::vec3{0.0f, .1f, .00f};
    }
    if (key == GLFW_KEY_E)
    {
        m_Camera.Position -= glm::vec3{.00f, .1f, .00f};
    }
    if (key == GLFW_KEY_W)
    {
        m_Camera.Position -= glm::vec3{.00f, .00f, .1f};
    }
    if (key == GLFW_KEY_S)
    {
        m_Camera.Position += glm::vec3{.00f, .00f, .1f};
    }
}
