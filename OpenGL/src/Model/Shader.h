#pragma once
#include <map>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace OpenGL
{
    class Shader
    {
    public:
        Shader();
        ~Shader();
        void AttachShader(const std::string& fileName, uint32_t shaderType) const;
        void Link() const;
        void Use() const;
        void AddUniform(const std::string& varName);

        void setFloat(const std::string& varName, float value);
        void set3Float(const std::string& varName, float val1, float val2, float val3);

        void setVec3(const std::string& varName, const glm::vec3& value);
        void setVec4(const std::string& varName, const glm::vec4& value);

        void setMat3(const std::string& varName, const glm::mat3* ptrValue);
        void setMat4(const std::string& varName, const glm::mat4* ptrValue);

    private:
        uint32_t                        m_ProgramId;
        std::map<std::string, uint32_t> m_UniformVars;

        static std::string getShaderFromFile(const std::string& fileName);
        static bool        check_error(uint32_t shaderId, int type);
        static bool        check_linking_error(uint32_t programId);
    };
}
