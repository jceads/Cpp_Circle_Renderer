#include "Shader.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include "GL/glew.h"

OpenGL::Shader::Shader()
{
    m_ProgramId = glCreateProgram();
}

OpenGL::Shader::~Shader()
{
    glDeleteProgram(m_ProgramId);
}


void OpenGL::Shader::AttachShader(const std::string& fileName, uint32_t shaderType) const
{
    const uint32_t shaderId = glCreateShader(shaderType);
    const auto     src      = getShaderFromFile(fileName);
    const char*    c_src    = &src[0];
    glShaderSource(shaderId, 1, &c_src, 0);
    glCompileShader(shaderId);
    if (!check_error(shaderId, GL_COMPILE_STATUS))
    {
        exit(-1);
    }
    glAttachShader(m_ProgramId, shaderId);
    glDeleteShader(shaderId);
}

bool OpenGL::Shader::check_error(uint32_t shaderId, int type)
{
    char log[512];
    int  isCompiled;
    glGetShaderiv(shaderId, type, &isCompiled);
    if (!isCompiled)
    {
        glGetShaderInfoLog(shaderId, 512, 0, log);
        std::cout << "Shader Type:" << type << "\n log: " << log << std::endl;
        return false;
    }
    return true;
}

bool OpenGL::Shader::check_linking_error(uint32_t programId)
{
    int  isLinked;
    char log[512];
    glGetProgramiv(programId,GL_LINK_STATUS, &isLinked);
    if (!isLinked)
    {
        glGetProgramInfoLog(programId, 512, 0, log);
        std::cout << "Linking Error:" << std::endl << log << std::endl;
        return false;
    }
    return true;
}


void OpenGL::Shader::Link() const
{
    glLinkProgram(m_ProgramId);
    if (!check_linking_error(m_ProgramId))
    {
        std::cin.get();
        exit(-1);
    }
}

void OpenGL::Shader::Use() const
{
    glUseProgram(m_ProgramId);
}

void OpenGL::Shader::AddUniform(const std::string& varName)
{
    m_UniformVars[varName] = glGetUniformLocation(m_ProgramId, varName.c_str());
}

void OpenGL::Shader::setFloat(const std::string& varName, float value)
{
    glUniform1f(m_UniformVars[varName], value);
}

void OpenGL::Shader::set3Float(const std::string& varName, float val1, float val2, float val3)
{
    glUniform3f(m_UniformVars[varName], val1, val2, val3);
}

void OpenGL::Shader::setVec3(const std::string& varName, const glm::vec3& value)
{
    glUniform3f(m_UniformVars[varName], value.x, value.y, value.z);
}

void OpenGL::Shader::setVec4(const std::string& varName, const glm::vec4& value)
{
    glUniform4f(m_UniformVars[varName], value.r, value.g, value.b, value.a);
}

void OpenGL::Shader::setMat3(const std::string& varName, const glm::mat3* ptrValue)
{
    glUniformMatrix3fv(m_UniformVars[varName], 1, false, (float*)ptrValue);
}

void OpenGL::Shader::setMat4(const std::string& varName, const glm::mat4* ptrValue)
{
    glUniformMatrix4fv(m_UniformVars[varName], 1, false, (GLfloat*)ptrValue);
}

std::string OpenGL::Shader::getShaderFromFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    std::string   data;
    if (file.is_open())
    {
        char readChar;
        while ((readChar = file.get()) != EOF)
        {
            data += readChar;
        }

        file.close();
    }
    return data;
}
