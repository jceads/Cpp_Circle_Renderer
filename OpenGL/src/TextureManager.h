#pragma once
#include <map>
#include <string>
#define STB_IMAGE_IMPLEMENTATION


namespace OpenGL
{
    class TextureManager
    {
    public:
        TextureManager() = default;
        unsigned int LoadTexture(std::string fileName);
        void         ActivateTexture(unsigned int level, unsigned int id);

    private:
        std::map<std::string, unsigned int> m_TextureMap;
    };
}
