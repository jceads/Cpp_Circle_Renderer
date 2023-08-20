#include "Scene.h"

OpenGL::Scene::Scene()
{
    m_VaoManager     = new VAOManager();
    m_MeshManager    = new MeshManager(m_VaoManager);
    m_TextureManager = new TextureManager();
}

OpenGL::Scene::~Scene()
{
    delete m_MeshManager;
    delete m_TextureManager;
}

OpenGL::MeshManager* OpenGL::Scene::GetMeshManager() const
{
    return m_MeshManager;
}

OpenGL::TextureManager* OpenGL::Scene::GetTextureManager() const
{
    return m_TextureManager;
}

OpenGL::VAOManager* OpenGL::Scene::GetVaoManager() const
{
    return m_VaoManager;
}
