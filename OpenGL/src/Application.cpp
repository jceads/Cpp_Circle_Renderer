#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "Mesh.h"
#include "Shader.h"
#include "stb/stb_image.h"
//#include "../../Dependencies/include/stb/stb_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

enum class Directions { up, down };

Directions currentDir{Directions::up};
//funcs
void drop_callback(GLFWwindow* window, int path_count, const char* paths[]);
void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void generateIndices(std::vector<unsigned int>& indexList, const int triangleCount);
bool compare_float(float x, float y, float epsilon = 0.1f);
void do_animation(Directions& currentDir);
void do_rotation(glm::mat4& mtxTransform, OpenGL::Shader& m_Shader);

//variables
int      wWidth, wHeight;
uint32_t VBO,    VAO, EBO, Texture;

std::vector<std::string> dragAndDroppedFiles;


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
    wHeight = 600;
    wWidth  = 800;


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


    int                  width, height, nrChannels;
    unsigned char*       textureData  = stbi_load("./data/images/container.jpg", &width, &height, &nrChannels, 0);
    OpenGL::MeshManager* mesh_manager = OpenGL::MeshManager::GetInstance();
    OpenGL::Mesh*        mesh         = mesh_manager->createCube();


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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    float fov       = 90.0f;
    float nearPlane = 0.1f, farPlane = 1000.0f;
    // const glm::mat4 mtxProjection(
    //     glm::perspective(glm::radians(fov),
    //                      static_cast<float>(wWidth) / static_cast<float>(wHeight),
    //                      nearPlane, farPlane)
    // );

    constexpr glm::vec3 camPosition{2.0f, 2.0f, 2.0f};
    constexpr glm::vec3 cameraLookAt{0.0f, 0.0f, 0.0f};
    constexpr glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
    const glm::mat4     mtxCamera = glm::lookAt(camPosition, cameraLookAt, cameraUp);
    float               angle     = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        ImGui::SliderFloat("rotation", &angle, 0, 360);
        ImGui::SliderFloat("Near Plane", &nearPlane, 0.01f, 120.0f);
        ImGui::SliderFloat("far Plane", &farPlane, 100.0f, 2000.0f);
        ImGui::SliderFloat("Field of View", &fov, 30.0f, 120.0f);

        for (const auto& item : dragAndDroppedFiles)
        {
            ImGui::Text(item.c_str());
        }

        glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Shader.Use();
        glBindTexture(GL_TEXTURE_2D, Texture);
        mesh->Draw();
        glm::mat4 mtxRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));

        glm::mat4 mtxProjection = glm::perspective(glm::radians(fov),
                                                   static_cast<float>(wWidth) / static_cast<float>(wHeight), nearPlane,
                                                   farPlane);
        mtxTransform = mtxProjection * mtxCamera * mtxRotation;
        m_Shader.setMat4("uMtxTransform", &mtxTransform);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete mesh;
    delete mesh_manager;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
    const glm::mat4 mtxTranslation = glm::translate(glm::mat4{1}, position);
    transform                      = mtxTranslation; //* mtxRotation * mtxScale;
    m_rotation_angle += 1.0f;
}


bool compare_float(float x, float y, float epsilon)
{
    return fabs(x - y) < epsilon;
}


void frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    wWidth  = width;
    wHeight = height;
    glViewport(0, 0, width, height);
    glfwSetWindowSize(window, width, height);
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
    {
        dragAndDroppedFiles.push_back(std::string(paths[i]));
    }
    // std::cout << paths[i] << std::endl;
}
