#include "Mesh.h"
#include "GL/glew.h"


void OpenGL::Mesh::Draw() const
{
    m_Vao->Activate();

    glDrawElements(GL_TRIANGLES, m_IndexCount,GL_UNSIGNED_INT, 0);
    m_Vao->DeActivate();
}
