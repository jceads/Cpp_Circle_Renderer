#include "WindowManager.h"

int main()
{
    OpenGL::WindowManager window_manager;
    window_manager.Run();
    window_manager.Dispose();

    return 0;
}
