#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Utils/IDisposable.h"

namespace OpenGL
{
    class Scene;

    class WindowManager : public IDisposable
    {
    public:
        WindowManager();
        ~WindowManager() = default;
        void              Run();
        void              Dispose() override;
        inline static int m_width  = 1000;
        inline static int m_height = 1000;

    private:
        static void DragAndDropCallBack(GLFWwindow* window, int path_count, const char* paths[]);
        static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
        static void GlfwError(int id, const char* description);
        void        ClearColor() const;
        void        LoopEndFuncs() const;
        GLFWwindow* m_window;
        int         m_swapInterval = 0;
        Scene*      m_scene;
    };
}
