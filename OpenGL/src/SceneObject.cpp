#include "SceneObject.h"

#include "GL/glew.h"

OpenGL::SceneObject::SceneObject()
{
    m_shader  = new Shader();
    m_mesh    = new Mesh();
    m_texture = new Texture();

    m_shader->AttachShader("./data/shaders/vertex.glsl",GL_VERTEX_SHADER);
    m_shader->AttachShader("./data/shaders/fragment.glsl",GL_FRAGMENT_SHADER);
    m_shader->Link();
}

OpenGL::SceneObject::~SceneObject()
{
    delete m_shader;
    delete m_mesh;
    delete m_texture;
}

void OpenGL::SceneObject::Draw()
{
    m_shader->Use();
    m_mesh->Draw();
    if (!m_texture)
    {
        m_texture->ActivateTexture();
    }
}
