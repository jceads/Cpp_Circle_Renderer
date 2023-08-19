#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "Model/Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../include/VertexArrayObject.h"

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
void do_rotation(glm::mat4& mtxTransform, OpenGL::Shader& m_Shader);

//variables
int                         wWidth, wHeight;
uint32_t                    VBO,    VAO, EBO, Texture;
std::vector<OpenGL::Vertex> m_Vertices;
std::vector<unsigned int>   m_Indices;


//matrix related
float shaderMoveValue{0.f}, m_rotation_angle{1.0f}, scale{1.f};
//--
float     keyStrength{2.f};
glm::vec3 position;
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
    int                       width, height, nrChannels;
    unsigned char*            textureData = stbi_load("./data/images/container.jpg", &width, &height, &nrChannels, 0);
    OpenGL::VertexArrayObject vao;
    vao.Build(m_Vertices, m_Indices);


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

    glm::mat4 mtxTransform{m_rotation_angle};

    position = glm::vec3(0.0f, 0.0f, 0.0f);


    glEnable(GL_DEPTH_TEST);

    //MAIN LOOP

    glm::mat4 mtxProjection{glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 1000.0f)};

    glm::vec3 camPosition{0.0f, 0.0f, 2.0f};
    glm::vec3 cameraLookAt{0.0f, 0.0f, 0.0f};
    glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
    glm::mat4 mtxCamera = glm::lookAt(camPosition, cameraLookAt, cameraUp);
    float     angle     = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // do_rotation(mtxTransform, m_Shader);
        m_Shader.Use();

        // glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // glBindVertexArray(VAO);
        vao.Activate();
        glDrawElements(GL_TRIANGLES, m_Indices.size(),GL_UNSIGNED_INT, 0);

        glm::mat4 mtxRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));

        angle += 0.2f;

        // glm::mat4 mtxTranslation = glm::translate(glm::mat4(1), position);
        // mtxTransform             = mtxProjection * mtxCamera * mtxTranslation;
        mtxTransform = mtxProjection * mtxCamera * mtxRotation;
        m_Shader.setMat4("uMtxTransform", &mtxTransform);

        // do_animation(currentDir);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.DeActivate();
    glfwTerminate();
    return 0;
}

void moveCamera(int key, glm::vec3& camPos)
{
    if (key == GLFW_KEY_A)
    {
        camPos.x -= 0.1f * keyStrength;
    }
    if (key == GLFW_KEY_A)
    {
        camPos.x -= 0.1f * keyStrength;
    }
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


    OpenGL::Vertex tempVertices[24];
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

void do_rotation(glm::mat4& transform, OpenGL::Shader& shader)
{
    shader.setMat4("uMtxTransform", &transform);
    // const glm::mat4 mtxRotation    = rotate(glm::mat3{1}, glm::radians(m_rotation_angle));
    const glm::mat4 mtxTranslation = glm::translate(glm::mat4{1}, position);
    // const glm::mat4 mtxScale       = glm::scale(glm::mat3{1}, glm::vec2{scale, scale});
    transform = mtxTranslation; //* mtxRotation * mtxScale;
    m_rotation_angle += 1.0f;
}


bool compare_float(float x, float y, float epsilon)
{
    return fabs(x - y) < epsilon;
}

void makeCircle(float radius, int vertexCount)
{
    const float            angle         = 360.0f / vertexCount;
    const int              triangleCount = vertexCount - 2;
    std::vector<glm::vec3> tempVertices;
    for (int i = 0; i < vertexCount; i++)
    {
        const float newAngle = glm::radians(angle * i);
        m_Vertices.emplace_back(
            OpenGL::Vertex{
                glm::vec3(radius * cos(newAngle),
                          radius * sin(newAngle),
                          1.0f)
            }
        );
    }

    generateIndices(m_Indices, triangleCount);
}


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
