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
    m_height = height;
    m_width  = width;
    GlCall(glViewport(0, 0, m_width, m_height))
    GlCall(glfwSetWindowSize(window, width, height))
}

void OpenGL::WindowManager::GlfwError(int id, const char* description)
{
    std::cout << description << std::endl;
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
    m_width  = 1000;
    m_height = 1000;
    glfwSetErrorCallback(GlfwError);
    if (!glfwInit())
    {
        std::cout << "Error: Glfw cant initialized";
        exit(-1);
    }

    m_window = glfwCreateWindow(m_width, m_height, "Opengl Renderer", nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        std::cout << "Failed to load window";
        exit(-1);
    }

    glfwMakeContextCurrent(m_window);
    glfwSetDropCallback(m_window, DragAndDropCallBack);
    glfwSetKeyCallback(m_window, KeyCallBack);
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

OpenGL::WindowManager::~WindowManager()
{
    WindowManager::Dispose();
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
        m_scene->Draw();
        LoopEndFuncs();
    }
}

void OpenGL::WindowManager::Dispose()
{
    IDisposable::Dispose();
    // delete m_scene;
    glfwTerminate();
}
