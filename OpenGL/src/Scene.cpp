#include "Scene.h"

#include <iostream>

#include "Model.h"
#include "assimp/postprocess.h"


OpenGL::Scene::Scene()
{
    AddCube();
}


void OpenGL::Scene::Draw()
{
    for (const auto& element : m_SceneMeshes)
    {
        element->Draw(&m_camera);
    }
}

void OpenGL::Scene::AddCube()
{
    std::string path{"./data/ico_sphere.fbx"};
    auto*       mesh = new Model(path, false);
    m_SceneMeshes.push_back(mesh);
}
