#pragma once
#include "Shader.h"


namespace OpenGL
{
    class Renderable
    {
    public:
        virtual      ~Renderable() = default;
        virtual void Draw(OpenGL::Shader& shader);
        unsigned int VAO;
    };
}
