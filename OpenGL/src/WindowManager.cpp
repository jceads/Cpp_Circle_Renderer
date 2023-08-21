#include "WindowManager.h"
#include <cstdlib>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#ifndef GLGuard
#define GLGuard

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
x;\
ASSERT(GlLogCall(#x, __FILE__,__LINE__))

static void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* func, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Opengl Error] (" << error << ") " << func << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}
#endif


void OpenGL::WindowManager::DragAndDropCallBack(GLFWwindow* window, int path_count, const char* paths[])
{
    std::cout << "path count " << path_count << std::endl;
    for (int i = 0; i < path_count; i++)
    {
        std::cout << paths[i] << std::endl;
        // dragAndDroppedFiles.push_back(std::string(paths[i]));
    }
}

void OpenGL::WindowManager::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            exit(0);
        }
    }
}

void OpenGL::WindowManager::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    GlCall(glViewport(0, 0, width, height))
    GlCall(glfwSetWindowSize(window, width, height))
}

void OpenGL::WindowManager::GlfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

void OpenGL::WindowManager::ScrollCallBack(GLFWwindow* window, double xpos, double ypos)
{
}

void OpenGL::WindowManager::ClearColor() const
{
    GlCall(glClearColor(0.3f, 0.2f, 0.5f, 1.0f))
    GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
}

void OpenGL::WindowManager::LoopEndFuncs() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    GlCall(glfwSwapBuffers(m_window);)
    GlCall(glfwPollEvents();)
}


OpenGL::WindowManager::WindowManager()
{
    glfwSetErrorCallback(GlfwError);
    if (!glfwInit())
    {
        std::cout << "Error: Glfw cant initialized";
        exit(-1);
    }

    m_window = glfwCreateWindow(1000, 1000, "Opengl Renderer", nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        std::cout << "Failed to load window";
        exit(-1);
    }

    glfwMakeContextCurrent(m_window);
    glfwSetDropCallback(m_window, DragAndDropCallBack);
    glfwSetKeyCallback(m_window, KeyCallBack);
    glfwSetScrollCallback(m_window, ScrollCallBack);
    glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallBack);
    glfwSwapInterval(m_swapInterval);
    // stbi_set_flip_vertically_on_load(true);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew Error!" << std::endl;
    }

    std::cout << "Device is: " << glGetString(GL_VERSION) << std::endl;
    GlCall(glEnable(GL_DEPTH_TEST))


    m_scene = new Scene();
}

void OpenGL::WindowManager::Run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ClearColor();
        // GlClearError();
        m_scene->Draw();
        // GlLogCall();
        LoopEndFuncs();
    }
}

void OpenGL::WindowManager::Dispose()
{
    IDisposable::Dispose();
    delete m_scene;
    glfwTerminate();
}
