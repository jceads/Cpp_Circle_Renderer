#pragma once
#include <map>
#include <string>

#include "VertexArrayObject.h"

namespace OpenGL
{
    class VAOManager
    {
    public:
        VAOManager()  = default;
        ~VAOManager() = default;
        void               AddNewObject(std::string name, VertexArrayObject* vao);
        VertexArrayObject* GetVao(std::string name);

    private:
        std::map<std::string, VertexArrayObject*> m_VaoMap;
    };
}
