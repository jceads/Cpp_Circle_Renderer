#include "Model.h"
#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "stb/stb_image.h"

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
 * \param path Model loading path ex: C:/etc/..
 * \param gamma true by default
 */
OpenGL::Model::Model(const std::string& path, bool gamma = false) : gammCorrection(gamma)
{
    shader = new Shader("./data/shaders/vertex.glsl", "./data/shaders/fragment.glsl");
    LoadModel(path);
}

void OpenGL::Model::Draw(Camera* camera)
{
    for (unsigned int i = 0; i < Meshes.size(); ++i)
    {
        shader->Use();
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)1000 / (float)1000, 0.1f, 100.0f);
        glm::mat4 view       = camera->GetViewMatrix();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale it down
        shader->setMat4("model", model);

        Meshes[i].Draw(*shader);
    }
}

void OpenGL::Model::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene*   scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        std::cin.get();
        return;
    }
    Directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void OpenGL::Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        auto* mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

OpenGL::Mesh OpenGL::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex    vertex;
        glm::vec3 temp_vector;
        temp_vector.x   = mesh->mVertices[i].x;
        temp_vector.y   = mesh->mVertices[i].y;
        temp_vector.z   = mesh->mVertices[i].z;
        vertex.Position = temp_vector;

        if (mesh->HasNormals())
        {
            temp_vector.x = mesh->mNormals[i].x;
            temp_vector.y = mesh->mNormals[i].y;
            temp_vector.z = mesh->mNormals[i].z;
            vertex.Normal = temp_vector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // // tangent
            // temp_vector.x = mesh->mTangents[i].x;
            // temp_vector.y = mesh->mTangents[i].y;
            // temp_vector.z = mesh->mTangents[i].z;
            // vertex.Tangent = temp_vector;
            // // bitangent
            // temp_vector.x = mesh->mBitangents[i].x;
            // temp_vector.y = mesh->mBitangents[i].y;
            // temp_vector.z = mesh->mBitangents[i].z;
            // vertex.BiTangent = temp_vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial*          material    = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    return Mesh(vertices, indices, textures);
}

std::vector<OpenGL::Texture> OpenGL::Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                                                 std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id   = TextureFromFile(str.C_Str(), Directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned OpenGL::Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename             = directory + '/' + filename;

    unsigned int textureID;
    GlCall(glGenTextures(1, &textureID))

    int            width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        GlCall(glBindTexture(GL_TEXTURE_2D, textureID))
        GlCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data))
        GlCall(glGenerateMipmap(GL_TEXTURE_2D))

        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
        GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
