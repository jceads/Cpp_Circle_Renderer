#pragma once
#include <iostream>

namespace OpenGL
{
    class InputListener
    {
    public:
        virtual ~InputListener() = default;

        virtual void Notify(int key, int action)
        {
            std::cout << "Key Pressed: " << key << " Action is: " << action << std::endl;
        }
    };
}
