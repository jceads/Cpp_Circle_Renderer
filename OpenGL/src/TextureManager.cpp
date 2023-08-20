#include "TextureManager.h"
#include "GL/glew.h"
#include "stb/stb_image.h"


unsigned OpenGL::TextureManager::LoadTexture(std::string fileName)
{
    unsigned int id = -1;
    if (m_TextureMap.count(fileName) != 0)
    {
        id = m_TextureMap[fileName];
    }
    else
    {
        int            width, height, nrChannels;
        unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        m_TextureMap[fileName] = id;
    }
    return id;
}

void OpenGL::TextureManager::ActivateTexture(unsigned level, unsigned id)
{
    glBindTexture(GL_TEXTURE_2D, id);
    glActiveTexture(level);
}
