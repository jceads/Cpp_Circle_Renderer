#include "WindowManager.h"

#include <cstdlib>
#include <iostream>

#include "Scene.h"

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
    // m_width  = width;
    // m_height = height;
    glViewport(0, 0, width, height);
    glfwSetWindowSize(window, width, height);
}

void OpenGL::WindowManager::GlfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

void OpenGL::WindowManager::ClearColor() const
{
    glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::WindowManager::LoopEndFuncs() const
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}


OpenGL::WindowManager::WindowManager()
{
    glfwSetErrorCallback(GlfwError);
    if (!glfwInit())
        exit(-1);
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
    glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallBack);
    glfwSwapInterval(m_swapInterval);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew Error!" << std::endl;
    }

    std::cout << "Device is: " << glGetString(GL_VERSION) << std::endl;


    m_scene = new Scene();
}

void OpenGL::WindowManager::Run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        ClearColor();
        m_scene->Draw();
        LoopEndFuncs();
    }
}

void OpenGL::WindowManager::Dispose()
{
    IDisposable::Dispose();
    delete m_scene;
    glfwTerminate();
}
