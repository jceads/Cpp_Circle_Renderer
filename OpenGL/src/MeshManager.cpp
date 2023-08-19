#include "MeshManager.h"

#include <vector>
#include <glm/vec3.hpp>

#include "Model/Mesh.h"
#include "Model/Vertex.h"
#include "Model/VertexArrayObject.h"
OpenGL::MeshManager* OpenGL::MeshManager::m_Instance = nullptr;

OpenGL::MeshManager* OpenGL::MeshManager::GetInstance()
{
    if (m_Instance == nullptr)
    {
        m_Instance = new MeshManager();
        return m_Instance;
    }
    return m_Instance;
}

OpenGL::Mesh* OpenGL::MeshManager::createCube()
{
    std::vector<Vertex>       m_Vertices;
    std::vector<unsigned int> m_Indices;
    float                     length = 1.0f;
    // OpenGL::Vertex v0, v1, v2, v3, v4, v5, v6, v7;
    glm::vec3 v[8];
    // v0.position = glm::vec3(-length / 2, length / 2, 0.0f);
    // v1.position = glm::vec3(-length / 2, -length / 2, 0.0f);
    // v2.position = glm::vec3(length / 2, -length / 2, 0.0f);
    // v3.position = glm::vec3(length / 2, length / 2, 0.0f);

    v[0] = glm::vec3(-length / 2, -length / 2, -length / 2);
    v[1] = glm::vec3(length / 2, -length / 2, -length / 2);
    v[2] = glm::vec3(length / 2, -length / 2, length / 2);
    v[3] = glm::vec3(-length / 2, -length / 2, length / 2);

    v[4] = glm::vec3(-length / 2, length / 2, -length / 2);
    v[5] = glm::vec3(length / 2, length / 2, -length / 2);
    v[6] = glm::vec3(length / 2, length / 2, length / 2);
    v[7] = glm::vec3(-length / 2, length / 2, length / 2);


    Vertex tempVertices[24];
    tempVertices[0].position = v[7];
    tempVertices[1].position = v[3];
    tempVertices[2].position = v[2];
    tempVertices[3].position = v[6];

    tempVertices[0].texture = glm::vec2(0.0f, 1.0f);
    tempVertices[1].texture = glm::vec2(0.0f, 0.0f);
    tempVertices[2].texture = glm::vec2(1.0f, 0.0f);
    tempVertices[3].texture = glm::vec2(1.0f, 1.0f);

    tempVertices[4].position = v[6];
    tempVertices[5].position = v[2];
    tempVertices[6].position = v[1];
    tempVertices[7].position = v[5];
    tempVertices[4].texture  = glm::vec2(0.0f, 1.0f);
    tempVertices[5].texture  = glm::vec2(0.0f, 0.0f);
    tempVertices[6].texture  = glm::vec2(1.0f, 0.0f);
    tempVertices[7].texture  = glm::vec2(1.0f, 1.0f);
    //    //Ust Yuzey
    tempVertices[8].position  = v[4];
    tempVertices[9].position  = v[7];
    tempVertices[10].position = v[6];
    tempVertices[11].position = v[5];
    tempVertices[8].texture   = glm::vec2(0.0f, 1.0f);
    tempVertices[9].texture   = glm::vec2(0.0f, 0.0f);
    tempVertices[10].texture  = glm::vec2(1.0f, 0.0f);
    tempVertices[11].texture  = glm::vec2(1.0f, 1.0f);

    //Sol Yüzey
    tempVertices[12].position = v[0];
    tempVertices[13].position = v[3];
    tempVertices[14].position = v[2];
    tempVertices[15].position = v[1];
    tempVertices[12].texture  = glm::vec2(0.0f, 1.0f);
    tempVertices[13].texture  = glm::vec2(0.0f, 0.0f);
    tempVertices[14].texture  = glm::vec2(1.0f, 0.0f);
    tempVertices[15].texture  = glm::vec2(1.0f, 1.0f);
    //Sag Yuzey
    tempVertices[16].position = v[4];
    tempVertices[17].position = v[0];
    tempVertices[18].position = v[1];
    tempVertices[19].position = v[5];
    tempVertices[16].texture  = glm::vec2(0.0f, 1.0f);
    tempVertices[17].texture  = glm::vec2(0.0f, 0.0f);
    tempVertices[18].texture  = glm::vec2(1.0f, 0.0f);
    tempVertices[19].texture  = glm::vec2(1.0f, 1.0f);
    //    //Alt Yuzey
    tempVertices[20].position = v[7];
    tempVertices[21].position = v[3];
    tempVertices[22].position = v[0];
    tempVertices[23].position = v[4];
    tempVertices[20].texture  = glm::vec2(0.0f, 1.0f);
    tempVertices[21].texture  = glm::vec2(0.0f, 0.0f);
    tempVertices[22].texture  = glm::vec2(1.0f, 0.0f);
    tempVertices[23].texture  = glm::vec2(1.0f, 1.0f);

    for (int i = 0; i < 24; ++i)
    {
        m_Vertices.push_back(tempVertices[i]);
    }
    for (int i = 0; i < 6; ++i)
    {
        int startIndex = 4 * i;
        m_Indices.push_back(startIndex);
        m_Indices.push_back(startIndex + 1);
        m_Indices.push_back(startIndex + 1);

        m_Indices.push_back(startIndex);
        m_Indices.push_back(startIndex + 2);
        m_Indices.push_back(startIndex + 3);
    }
    auto* vao = new VertexArrayObject();
    vao->Build(m_Vertices, m_Indices);

    auto* mesh         = new Mesh();
    mesh->m_Vao        = vao;
    mesh->m_IndexCount = m_Indices.size();

    return mesh;
}

OpenGL::MeshManager::~MeshManager()
{
    delete m_Instance;
}
