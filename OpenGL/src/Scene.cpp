#include "Scene.h"

#include <iostream>

#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include "assimp/postprocess.h"

// OpenGL::Scene::Scene()
// {
//     m_VaoManager     = new VAOManager();
//     m_MeshManager    = new MeshManager(m_VaoManager);
//     m_TextureManager = new Texture();
// }

OpenGL::Scene::Scene()
{
    AddCube();
}

// OpenGL::Scene::~Scene()
// {
//     delete m_MeshManager;
//     delete m_TextureManager;
// }
//
// OpenGL::MeshManager* OpenGL::Scene::GetMeshManager() const
// {
//     return m_MeshManager;
// }
//
// OpenGL::Texture* OpenGL::Scene::GetTextureManager() const
// {
//     return m_TextureManager;
// }
//
// OpenGL::VAOManager* OpenGL::Scene::GetVaoManager() const
// {
//     return m_VaoManager;
// }
//
void OpenGL::Scene::Draw()
{
    for (auto& element : m_sceneObjects)
    {
        element.Draw();
    }
}

void OpenGL::Scene::AddCube()
{
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile("./data/ico_sphere.fbx",
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

    // If the import failed, report it
    if (nullptr == scene)
    {
        std::cout << "Error " << importer.GetErrorString();
    }

}
