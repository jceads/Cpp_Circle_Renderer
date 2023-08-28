#pragma once
#include <vector>
#include "InputListener.h"

namespace OpenGL
{
    class KeyInputManager
    {
    public:
        static void NotifyListeners(int key, int action);
        static void AddListener(InputListener* listener);

    private:
        static std::vector<InputListener*> m_inputListeners;
    };
}
