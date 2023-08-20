#pragma once

namespace OpenGL
{
    class IDisposable
    {
    public:
        // virtual      ~IDisposable() = default;
        virtual void Dispose();
    };
}
