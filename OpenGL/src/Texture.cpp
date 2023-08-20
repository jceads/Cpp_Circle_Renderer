#include "Texture.h"
#include "GL/glew.h"
// #include "imgui/imgui_impl_opengl3_loader.h"
#include "stb/stb_image.h"


OpenGL::Texture::Texture(std::string fileName)
{
    m_fileName = fileName;
    LoadTexture();
}

void OpenGL::Texture::LoadTexture()
{
    uint32_t id                = -1;
    auto*    read_texture_data = stbi_load(m_fileName.c_str(), &m_textureSpecs.width,
                                           &m_textureSpecs.height,
                                           &m_textureSpecs.nrChannels, 0);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D,
                 0,GL_RGB, m_textureSpecs.width,
                 m_textureSpecs.height, 0,GL_RGB,
                 GL_UNSIGNED_BYTE, read_texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(read_texture_data);
    m_TextureMap[m_fileName] = id;
}


unsigned OpenGL::Texture::LoadTexture(std::string fileName)
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

void OpenGL::Texture::ActivateTexture(unsigned level, unsigned id)
{
    glBindTexture(GL_TEXTURE_2D, id);
    glActiveTexture(level);
}

void OpenGL::Texture::ActivateTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_TextureMap[m_fileName]);
    glActiveTexture(GL_TEXTURE0);
}
