#include "Model.h"
#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#define STB_IMAGE_IMPLEMENTATION
#include "WindowManager.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"
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

void OpenGL::Model::AddAttribute3(const std::string& attrib_name, const glm::vec3* attrib)
{
    attrib_list3[attrib_name] = *attrib;
}

void OpenGL::Model::AddAttribute4(const std::string& attrib_name, const glm::vec4 attrib)
{
    attrib_list4[attrib_name] = attrib;
}

void OpenGL::Model::AddAttribute2(const std::string& attrib_name, glm::vec2 attrib)
{
    attrib_list2[attrib_name] = attrib;
}

void OpenGL::Model::AddAttributeF(const std::string& attrib_name, float value)
{
    attrib_listF[attrib_name] = value;
}

OpenGL::CameraProjection OpenGL::Model::calculateCameraProjection(Camera* camera)
{
    m_Camera_projection.projection = glm::perspective(glm::radians(camera->fov),
                                                      (float)WindowManager::window_properties.width / (float)
                                                      WindowManager::window_properties.height, camera->nearPlane,
                                                      camera->farPlane);
    m_Camera_projection.view  = camera->GetViewMatrix();
    m_Camera_projection.model = glm::mat4(1.0f);

    return m_Camera_projection;
}

/**
 * \brief 
 * \param path Model loading path ex: C:/etc/..
 * \param gamma true by default
 */
OpenGL::Model::Model(const std::string& path, bool gamma = false) : gammaCorrection(gamma), name("No name")
{
    // shader = new Shader("./data/shaders/vertex.glsl", "./data/shaders/fragment.glsl");
    LoadModel(path);
    // objectColor = glm::vec3{1.0f};
    // position    = glm::vec3{1.0f};
    // scale       = glm::vec3{1.0f};
}

OpenGL::Model::Model(const std::string& path) : name("No name")
{
    gammaCorrection = false;
    transform       = Transform{
        glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f}, glm::vec3{1.0f}

    };
    color = Color{1.0f};

    LoadModel(path);
}

void OpenGL::Model::Draw(Camera* camera, float* ambientStrength)
{
    calculateCameraProjection(camera);
    const glm::mat4 projection = m_Camera_projection.projection;
    const glm::mat4 view       = m_Camera_projection.view;
    glm::mat4       model      = m_Camera_projection.model;

    const std::string table_name            = "Mesh: " + name;
    const std::string attributes_table_name = "Attributes: " + name;
    ImGui::Begin(table_name.c_str());

    ImGui::DragFloat3("Position", &this->transform.position.x, 0.01f, -10.0f, 10.0f);
    ImGui::DragFloat3("Scale", &this->transform.scale.x, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat3("Object Color", &attrib_list3["objectColor"][0], 0.01f, 0.0f, 1.0f);

    ImGui::End();

    for (auto& mesh : Meshes)
    {
        shader->Use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        model = glm::translate(model, transform.position);
        model = glm::scale(model, transform.scale);
        shader->setMat4("model", model);
        glm::vec3 vec3Color = {this->color.r, this->color.g, this->color.b};

        ImGui::Begin(attributes_table_name.c_str());
        for (auto& [key,value] : attrib_list3)
        {
            ImGui::DragFloat3(key.c_str(), &value.x, 0.01f, 0.0f, 1.0f);
            shader->setVec3(key, value);
        }
        for (auto& [key , value] : attrib_listF)
        {
            ImGui::DragFloat(key.c_str(), &value, 0.01f, 0.0f, 1.0f);
            shader->setFloat(key, value);
        }
        ImGui::End();
        mesh.Draw(*shader);
    }
}

void OpenGL::Model::InitShader(const std::string& vPath, const std::string& fPath)
{
    shader = new Shader(vPath.c_str(), fPath.c_str());
}


void OpenGL::Model::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene*   scene = import.ReadFile(path, aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        std::cin.get();
        return;
    }
    Directory = path.substr(0, path.find_last_of('/'));
    name      = scene->mMeshes[0]->mName.C_Str();
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

void OpenGL::Model::Notify(int key, int action)
{
    InputListener::Notify(key, action);
    if (key == GLFW_KEY_RIGHT)
    {
        transform.position += glm::vec3{1.0f, 0, 0};
    }
}
