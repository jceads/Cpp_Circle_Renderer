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

//variables
int wWidth, wHeight;
uint32_t VBO, VAO, EBO;


//matrix related
float shaderMoveValue{0.f}, m_rotation_angle{1.0f}, scale{1.f};
//--
float keyStrength{2.f};
std::vector<glm::vec3> m_Vertices;
std::vector<unsigned int> m_Indices;
glm::vec2 position;
int sampleCount = 0;


int main()
{
    makeCircle(.5f, 12);

    glm::mat3 mtxTransform{m_rotation_angle};


    if (!glfwInit())
        return -1;
    wHeight = 1000;
    wWidth = 1000;
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


    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), &m_Vertices[0],GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0],GL_DYNAMIC_DRAW);


    //MAIN LOOP 

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.2f, 0.5f, 1.0f);

        m_Shader.Use();
        glDrawElements(GL_TRIANGLES, static_cast<int>(m_Indices.size()),GL_UNSIGNED_INT, 0);


        m_Shader.setMat3("uMtxTransform", &mtxTransform);
        glm::mat3 mtxRotation = rotate(glm::mat3{1}, glm::radians(m_rotation_angle));
        glm::mat3 mtxTranslation = glm::translate(glm::mat3{1}, position);
        glm::mat3 mtxScale = glm::scale(glm::mat3{1}, glm::vec2{scale, scale});
        mtxTransform = mtxTranslation * mtxRotation * mtxScale;
        m_rotation_angle += 1.0f;

        do_animation(currentDir);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void do_animation(Directions& currentDir)
{
    //anim
    switch (currentDir)
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
        currentDir = Directions::down;
    }
    if (compare_float(scale, 0.0f))
    {
        currentDir = Directions::up;
    }
    std::cout << "scale is " << scale << std::endl;
    //end anim
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

bool compare_float(float x, float y, float epsilon)
{
    return fabs(x - y) < epsilon;
}

void makeCircle(float radius, int vertexCount)
{
    const float angle = 360.0f / vertexCount;
    const int triangleCount = vertexCount - 2;
    std::vector<glm::vec3> tempVertices;
    for (int i = 0; i < vertexCount; i++)
    {
        const float newAngle = glm::radians(angle * i);
        m_Vertices.emplace_back(
            radius * cos(newAngle),
            radius * sin(newAngle),
            1.0f
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
