#pragma once
#include <map>
#include <string>
#define STB_IMAGE_IMPLEMENTATION


namespace OpenGL
{
    struct TextureSpecs
    {
        int width, height, nrChannels;
    };


    class Texture
    {
    public:
        Texture() = default;
        Texture(std::string fileName);
        unsigned int LoadTexture(std::string fileName);
        void         LoadTexture();
        void         ActivateTexture(unsigned int level, unsigned int id);
        void         ActivateTexture();

    private:
        std::map<std::string, unsigned int> m_TextureMap;
        std::string                         m_fileName;
        TextureSpecs                        m_textureSpecs;
    };
}
