#include "resources/model.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "resources/resource_manager.h"

#include "service_locator.h"

Model::Model()
{
    Init();
}

Model::Model(const char* path)
{
	LoadModel(path);

    Init();
}

void Model::Init()
{
    mRenderer = ServiceLocator::Get<Renderer>();

    mRenderer->GenerateVertexArray(1, &mVAO);
    mRenderer->GenerateBuffer(1, &mVBO);
    mRenderer->GenerateBuffer(1, &mEBO);

    mRenderer->BindBuffers(mVAO, mVBO, mEBO);

    // vertex positions
    mRenderer->AttributePointers(0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);

    // vertex normals
    mRenderer->AttributePointers(1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    mRenderer->AttributePointers(2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    mRenderer->BindBuffers(0, 0, 0);
}

bool Model::IsValid()
{
    return (mVAO != 00 && mVBO != 0 && mEBO != 0);
}

void Model::Draw(const Matrix4x4& TRS)
{
    mRenderer->BindBuffers(mVAO, mVBO, mEBO);

    for (std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>> pair : mModel)
    {
        mRenderer->UseShader(pair.second->MatShader->ID);
        mRenderer->SetUniform(pair.second->MatShader->ID, "model", TRS);
        mRenderer->SetBufferData(GL_ARRAY_BUFFER, (int)pair.first->Vertices.size() * sizeof(Vertex), &pair.first->Vertices[0], GL_STATIC_DRAW);
        mRenderer->SetBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)pair.first->Indices.size() * sizeof(unsigned int), &pair.first->Indices[0], GL_STATIC_DRAW);

        if (pair.second->MatTex)
        {
            mRenderer->BindTexture(pair.second->MatTex->GetID());
        }
        else
        {
            mRenderer->BindTexture(ResourceManager::Get<Texture>("assets/missing_texture.jpg")->GetID());
        }
        mRenderer->Draw(GL_TRIANGLES, (GLsizei)pair.first->Indices.size(), GL_UNSIGNED_INT, 0);
        mRenderer->UnUseShader();
    }
    
    mRenderer->BindBuffers(0, 0, 0);
    mRenderer->BindTexture(0);
}

void Model::SetTexture(int index, std::shared_ptr<Texture> tex)
{
    mModel[index].second->MatTex = tex;
}

void Model::LoadModel(const std::string& path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Logger::Error("ERROR::ASSIMP:: {} ", import.GetErrorString());
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mModel.push_back(std::make_pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>(std::make_shared<Mesh>(ProcessMesh(mesh)), std::make_shared<Material>(nullptr)));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh)
{
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // process vertex positions, normals and texture coordinates
        Vector3 v;
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;
        vertex.Position = v;

        v.x = mesh->mNormals[i].x;
        v.y = mesh->mNormals[i].y;
        v.z = mesh->mNormals[i].z;
        vertex.Normal = v;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            Vector2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = Vector2(0.0f, 0.0f);
        }

        Vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            Indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(Vertices, Indices);
}