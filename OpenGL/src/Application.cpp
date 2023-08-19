#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "Model/Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum class Directions { up, down };

Directions currentDir{Directions::up};
//funcs
void drop_callback(GLFWwindow* window, int path_count, const char* paths[]);
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void makeCircle(float radius, int vertexCount);
void generateIndices(std::vector<unsigned int>& indexList, const int triangleCount);
bool compare_float(float x, float y, float epsilon = 0.1f);
void do_animation(Directions& currentDir);
void buildSquare(float length);
void do_rotation(glm::mat3& mtxTransform, OpenGL::Shader& m_Shader);

//variables
int                         wWidth, wHeight;
uint32_t                    VBO,    VAO, EBO, Texture;
std::vector<OpenGL::Vertex> m_Vertices;
std::vector<unsigned int>   m_Indices;


//matrix related
float shaderMoveValue{0.f}, m_rotation_angle{1.0f}, scale{1.f};
//--
float     keyStrength{2.f};
glm::vec2 position;
int       sampleCount = 0;


int main()
{
    if (!glfwInit())
        return -1;
    wHeight = 1000;
    wWidth  = 1000;


    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetDropCallback(window, drop_callback);
    glfwSetKeyCallback(window, keyCallBack);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    std::cout << "Device is: " << glGetString(GL_VERSION) << std::endl;

    OpenGL::Shader m_Shader;
    m_Shader.AttachShader("./data/shaders/vertex.glsl",GL_VERTEX_SHADER);
    m_Shader.AttachShader("./data/shaders/fragment.glsl",GL_FRAGMENT_SHADER);
    m_Shader.Link();
    m_Shader.AddUniform("uMtxTransform");
    m_Shader.AddUniform("uColor");


    // makeCircle(.5f, 12);
    buildSquare(1.0f);
    int            width, height, nrChannels;
    unsigned char* textureData = stbi_load("./data/images/wood1.jpg", &width, &height, &nrChannels, 0);


    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    if (textureData)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            textureData);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    stbi_image_free(textureData);

    glm::mat3 mtxTransform{m_rotation_angle};


    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(OpenGL::Vertex) * m_Vertices.size(), m_Vertices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), m_Indices.data(),GL_STATIC_DRAW);


    //MAIN LOOP 

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        do_rotation(mtxTransform, m_Shader);
        m_Shader.Use();

        // glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(),GL_UNSIGNED_INT, 0);


        m_Shader.setVec4("uColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        // do_animation(currentDir);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void do_animation(Directions& dir)
{
    //anim
    switch (dir)
    {
    case Directions::up:
        scale += 0.05f;
        break;
    case Directions::down:
        scale -= 0.05f;
        break;
    }
    if (compare_float(scale, 2.0f))
    {
        dir = Directions::down;
    }
    if (compare_float(scale, 0.0f))
    {
        dir = Directions::up;
    }
    std::cout << "scale is " << scale << std::endl;
    //end anim
}

void buildSquare(float length)
{
    OpenGL::Vertex v0, v1, v2, v3;
    v0.position = glm::vec3(-length / 2, length / 2, 0.0f);
    v1.position = glm::vec3(-length / 2, -length / 2, 0.0f);
    v2.position = glm::vec3(length / 2, -length / 2, 0.0f);
    v3.position = glm::vec3(length / 2, length / 2, 0.0f);

    v0.texture = glm::vec2(0.0f, 1.0f);
    v1.texture = glm::vec2(0.0f, 0.0f);
    v2.texture = glm::vec2(1.0f, 0.0f);
    v3.texture = glm::vec2(1.0f, 1.0f);
    // v0.position = glm::vec3(0.5f, 0.5f, 0.0f);
    // v1.position = glm::vec3(0.5f, -0.5f, 0.0f);
    // v2.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    // v3.position = glm::vec3(-0.5f, 0.5f, 0.0f);
    //
    // v0.color = glm::vec3(1.0f, 0.0f, 0.0f);
    // v1.color = glm::vec3(0.0f, 1.0f, 0.0f);
    // v2.color = glm::vec3(0.0f, 0.0f, 1.0f);
    // v3.color = glm::vec3(1.0f, 1.0f, 0.0f);

    v0.texture = glm::vec2(1.0f, 1.0f);
    v1.texture = glm::vec2(1.0f, 0.0f);
    v2.texture = glm::vec2(0.0f, 0.0f);
    v3.texture = glm::vec2(0.0f, 1.0f);


    m_Vertices.push_back(v0);
    m_Vertices.push_back(v1);
    m_Vertices.push_back(v2);
    m_Vertices.push_back(v3);

    m_Indices.push_back(0);
    m_Indices.push_back(1);
    m_Indices.push_back(3);

    m_Indices.push_back(1);
    m_Indices.push_back(2);
    m_Indices.push_back(3);
}

void generateIndices(std::vector<unsigned int>& indexList, const int triangleCount)
{
    for (int i = 0; i < triangleCount; i++)
    {
        indexList.emplace_back(0);
        indexList.emplace_back(i + 1);
        indexList.emplace_back(i + 2);
    }
}

void do_rotation(glm::mat3& transform, OpenGL::Shader& shader)
{
    shader.setMat3("uMtxTransform", &transform);
    const glm::mat3 mtxRotation    = rotate(glm::mat3{1}, glm::radians(m_rotation_angle));
    const glm::mat3 mtxTranslation = glm::translate(glm::mat3{1}, position);
    const glm::mat3 mtxScale       = glm::scale(glm::mat3{1}, glm::vec2{scale, scale});
    transform                      = mtxTranslation * mtxRotation * mtxScale;
    m_rotation_angle += 1.0f;
}


bool compare_float(float x, float y, float epsilon)
{
    return fabs(x - y) < epsilon;
}

// void makeCircle(float radius, int vertexCount)
// {
//     const float            angle         = 360.0f / vertexCount;
//     const int              triangleCount = vertexCount - 2;
//     std::vector<glm::vec3> tempVertices;
//     for (int i = 0; i < vertexCount; i++)
//     {
//         const float newAngle = glm::radians(angle * i);
//         m_Vertices.emplace_back(
//             radius * cos(newAngle),
//             radius * sin(newAngle),
//             1.0f
//         );
//     }
//
//     generateIndices(m_Indices, triangleCount);
// }


void frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    if (width != wWidth)
    {
        wWidth = width;
        glViewport(0, 0, wWidth, wWidth);
        glfwSetWindowSize(window, wWidth, wWidth);
    }
    else if (wHeight != height)
    {
        wHeight = height;
        glViewport(0, 0, wHeight, wHeight);
        glfwSetWindowSize(window, wHeight, wHeight);
    }
}

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_Q)
        {
            scale += 0.1f * keyStrength;
        }
        if (key == GLFW_KEY_E)
        {
            scale -= 0.1f * keyStrength;
        }
    }

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwTerminate();
        exit(0);
    }
    if (key == GLFW_KEY_LEFT)
    {
        position.x -= 0.1f * keyStrength;
        std::cout << "Pressed to left btn" << std::endl;
        // shaderMoveValue -= 0.01f;
        // std::cout << "new pos is " << shaderMoveValue << std::endl;
    }
    if (key == GLFW_KEY_RIGHT)
    {
        position.x += 0.1f * keyStrength;
        std::cout << "Pressed to right btn" << std::endl;
        // shaderMoveValue += 0.01f;
    }
    if (key == GLFW_KEY_UP)
    {
        position.y += 0.1f * keyStrength;
        std::cout << "Pressed to up btn" << std::endl;

        // sampleCount++;
    }
    if (key == GLFW_KEY_DOWN)
    {
        position.y -= 0.1f * keyStrength;
        std::cout << "Pressed to down btn" << std::endl;
        // sampleCount--;
    }
    if (key == GLFW_KEY_Q)
    {
        scale += 0.1f * keyStrength;
    }
    if (key == GLFW_KEY_E)
    {
        scale -= 0.1f * keyStrength;
    }
}

void drop_callback(GLFWwindow* window, int path_count, const char* paths[])
{
    std::cout << "path count " << path_count << std::endl;
    for (int i = 0; i < path_count; i++)
        std::cout << paths[i] << std::endl;
}
