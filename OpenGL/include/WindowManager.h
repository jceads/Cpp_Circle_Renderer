#pragma once
#include "KeyInputManager.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Utils/IDisposable.h"
#include "Scene.h"
#include "WindowProperties.h"

namespace OpenGL
{
    class Scene;


    class WindowManager : public IDisposable
    {
    public:
        WindowManager();
        ~WindowManager() override;
        void                    Run();
        void                    Dispose() override;
        static WindowProperties window_properties;
        static KeyInputManager  m_inputManager;
        static void             DragAndDropCallBack(GLFWwindow* window, int path_count, const char* paths[]);
        static void             KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void             FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
        static void             CloseProgram();

    private:
        GLFWwindow*                m_window;
        glm::vec3                  bg_color;
        static std::vector<Scene*> m_scenes;

        static void GlfwError(int id, const char* description);
        void        ClearColor() const;
        void        LoopEndFuncs() const;
    };
}
