#include "Shader.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "GL/glew.h"
#include "imgui/imgui.h"

#ifndef GLGuard
#define GLGuard

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
x;\
ASSERT(GlLogCall(#x, __FILE__,__LINE__))

static void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* func, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Opengl Error] (" << error << ") " << func << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}
#endif


/**
 * \brief 
 * \param vertexPath Vertex shader file path
 * \param fragmentPath Fragment shader file path
 */
OpenGL::Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    R = 1.0f;
    G = 0.5f;
    B = 0.31f;

    LR = 1.0f;
    LG = 1.0f;
    LB = 1.0f;
    std::string   vertexCode;
    std::string   fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    GlCall(glAttachShader(ID, vertex);)
    GlCall(glAttachShader(ID, fragment);)
    GlCall(glLinkProgram(ID);)
    checkCompileErrors(ID, "PROGRAM");
    GlCall(glDeleteShader(vertex);)
    GlCall(glDeleteShader(fragment);)
}

OpenGL::Shader::~Shader()
{
    GlCall(glDeleteProgram(ID);)
}


void OpenGL::Shader::Use()
{
    GlCall(glUseProgram(ID);)
    // ImGui::Begin("Mesh color");
    // ImGui::DragFloat("Color R", &R, 0.001f, 0, 1.0f);
    // ImGui::DragFloat("Light R", &LR, 0.001f, 0, 1.0f);
    // setVec3("objectColor", R, G, B);
    // setVec3("lightColor", LR, 1.0f, 1.0f);
    // ImGui::End();
}


void OpenGL::Shader::setBool(const std::string& name, bool value) const
{
    GlCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);)
}

void OpenGL::Shader::setInt(const std::string& name, int value) const
{
    GlCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value);)
}

void OpenGL::Shader::setFloat(const std::string& name, float value) const
{
    GlCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value);)
}

void OpenGL::Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    GlCall(glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);)
}

void OpenGL::Shader::setVec2(const std::string& name, float x, float y) const
{
    GlCall(glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);)
}

void OpenGL::Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    GlCall(glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);)
}

void OpenGL::Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    GlCall(glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);)
}

void OpenGL::Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    GlCall(glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);)
}

void OpenGL::Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    GlCall(glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);)
}

void OpenGL::Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    GlCall(glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);)
}

void OpenGL::Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    GlCall(glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);)
}

void OpenGL::Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    auto loca = glGetUniformLocation(ID, name.c_str());
    GlCall(glUniformMatrix4fv(loca, 1, GL_FALSE, &mat[0][0]);)
}

void OpenGL::Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint  success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
