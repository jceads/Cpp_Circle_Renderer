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

std::vector<OpenGL::Scene*> OpenGL::WindowManager::m_scenes;
OpenGL::WindowProperties    OpenGL::WindowManager::window_properties = {
    1500, 1000, false, "OpenGL Renderer"
};

OpenGL::KeyInputManager OpenGL::WindowManager::m_inputManager;


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
    OpenGL::KeyInputManager::NotifyListeners(key, action);
}

void OpenGL::WindowManager::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    window_properties.height = height;
    window_properties.width  = width;
    glViewport(0, 0, window_properties.width, window_properties.height);
    glfwSetWindowSize(window, width, height);
}

void OpenGL::WindowManager::CloseProgram()
{
    glfwTerminate();
    exit(0);
}


void OpenGL::WindowManager::GlfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}


void OpenGL::WindowManager::ClearColor() const
{
    GlCall(glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.0f));
    GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
}

void OpenGL::WindowManager::LoopEndFuncs() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    GlCall(glfwSwapBuffers(m_window);)
    GlCall(glfwPollEvents();)
}


OpenGL::WindowManager::WindowManager(): bg_color({0.2f, 0.5f, 0.3f})
{
    window_properties.width        = 1500;
    window_properties.height       = 1000;
    window_properties.vSync        = true;
    window_properties.window_title = "Opengl Renderer";

    if (!glfwInit())
    {
        std::cout << "Error: Glfw cant initialized";
        exit(-1);
    }
    glfwSetErrorCallback(GlfwError);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


    m_window = glfwCreateWindow(window_properties.width, window_properties.height,
                                window_properties.window_title.c_str(), nullptr, nullptr);

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
    glfwSwapInterval(window_properties.vSync);

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


    m_scenes.push_back(new Scene());
    // m_scene = new Scene();
}

OpenGL::WindowManager::~WindowManager()
{
    WindowManager::Dispose();
}

void OpenGL::WindowManager::Run()
{
    bool        lastVsyncValue = window_properties.vSync;
    const auto& io             = ImGui::GetIO();
    while (!glfwWindowShouldClose(m_window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ClearColor();
        for (const auto& scene : m_scenes)
        {
            scene->Draw();
        }
        ImGui::Checkbox("Enable Vsync", &window_properties.vSync);
        if (lastVsyncValue != window_properties.vSync)
        {
            lastVsyncValue = window_properties.vSync;
            glfwSwapInterval(window_properties.vSync);
        }
        ImGui::Text("fps is %f", io.Framerate);
        ImGui::Separator();
        ImGui::ColorPicker4("BG Color", &bg_color[0]);
        LoopEndFuncs();
    }
}

void OpenGL::WindowManager::Dispose()
{
    IDisposable::Dispose();
    // delete m_scene;
    glfwTerminate();
}
