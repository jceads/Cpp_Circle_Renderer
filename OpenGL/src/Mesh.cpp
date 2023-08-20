#include "Mesh.h"
#include "GL/glew.h"


OpenGL::Mesh::~Mesh()
{
    m_Vao->DeActivate();
}

void OpenGL::Mesh::Draw() const
{
    m_Vao->Activate();

    glDrawElements(GL_TRIANGLES, m_IndexCount,GL_UNSIGNED_INT, 0);
}
