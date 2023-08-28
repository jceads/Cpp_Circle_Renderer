#pragma once
#include <string>


namespace OpenGL
{
    struct WindowProperties
    {
        int         width, height;
        bool        vSync;
        std::string window_title;
    };
}
