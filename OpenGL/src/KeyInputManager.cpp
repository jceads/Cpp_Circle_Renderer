#include "KeyInputManager.h"
#include <iostream>
#include <vector>
std::vector<OpenGL::InputListener*> OpenGL::KeyInputManager::m_inputListeners;

void OpenGL::KeyInputManager::NotifyListeners(int key, int action)
{
    for (const auto listener : m_inputListeners)
    {
        listener->Notify(key, action);
    }
}

void OpenGL::KeyInputManager::AddListener(InputListener* listener)
{
    m_inputListeners.push_back(listener);
}
