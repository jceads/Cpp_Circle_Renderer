#pragma once
#include <vector>
#include <string>

#include "Camera.h"
#include "Mesh.h"
#include "assimp/scene.h"


namespace OpenGL
{
    class Model
    {
    public:
        std::vector<OpenGL::Texture> textures_loaded;
        std::vector<Mesh>            Meshes;
        std::string                  Directory;
        bool                         gammCorrection;
        glm::vec3                    position;
        glm::vec3                    scale;
        std::string name;

        Model(const std::string& path, bool gamma);
        Model(const std::string& path);
        void    Draw(Camera* camera);
        void    InitShader(const std::string& vPath, const std::string& fPath);
        Shader* shader;

    private:
        void                 LoadModel(std::string path);
        void                 ProcessNode(aiNode* node, const aiScene* scene);
        Mesh                 ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        static unsigned int  TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
    };
}
