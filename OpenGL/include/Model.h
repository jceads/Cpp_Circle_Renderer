#pragma once
#include <map>
#include <vector>
#include <string>

#include "Camera.h"
#include "InputListener.h"
#include "Mesh.h"
#include "assimp/scene.h"
#include "Mesh/Color.h"
#include "Mesh/Texture.h"
#include "Mesh/Transform.h"


namespace OpenGL
{
    class Model : InputListener
    {
    public:
        std::vector<Texture>             textures_loaded;
        std::vector<Mesh>                Meshes;
        std::string                      Directory;
        bool                             gammaCorrection;
        void                             Draw(Camera* camera, float* ambientStrength);
        void                             InitShader(const std::string& vPath, const std::string& fPath);
        void                             AddAttributeF(const std::string& attrib_name, float value);
        void                             AddAttribute2(const std::string& attrib_name, glm::vec2 attrib);
        void                             AddAttribute3(const std::string& attrib_name, const glm::vec3* attrib);
        void                             AddAttribute4(const std::string& attrib_name, glm::vec4 attrib);
        std::map<std::string, float>     attrib_listF;
        std::map<std::string, glm::vec2> attrib_list2;
        std::map<std::string, glm::vec3> attrib_list3;
        std::map<std::string, glm::vec4> attrib_list4;
        Transform                        transform;
        Color                            color;
        std::string                      name;


        Model(const std::string& path, bool gamma);
        Model(const std::string& path);
        Shader* shader;

    private:
        CameraProjection     calculateCameraProjection(Camera* camera);
        CameraProjection     m_Camera_projection;
        void                 LoadModel(std::string path);
        void                 ProcessNode(aiNode* node, const aiScene* scene);
        Mesh                 ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        static unsigned int  TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

    public:
        void Notify(int key, int action) override;
    };
}
