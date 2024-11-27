#pragma once

#include <assimp/scene.h>

#include "resources/mesh.h"
#include "resources/material.h"

#include <refl.hpp>

class Renderer;
class ModelRenderer;

/// <summary>
/// Class Model that stores oen more meshes and materials
/// </summary>
class Model : public Resource
{
public:
    /// <summary>
    /// Default Constructor of Model
    /// </summary>
    UNDEFINED_ENGINE Model();
    /// <summary>
    /// Constructor of Model
    /// </summary>
    /// <param name="path">: Path to the file containg the Model data (e.g : obj)</param>
    UNDEFINED_ENGINE Model(const char* path);

    /// <summary>
    /// Initialise the Model
    /// </summary>
    UNDEFINED_ENGINE void Init();

    /// <summary>
    /// Check if the Model is valid
    /// </summary>
    /// <returns>Return either true if it is valid or false</returns>
    UNDEFINED_ENGINE bool IsValid();

    /// <summary>
    /// Set a Texture to a Mesh inside the model
    /// </summary>
    /// <param name="index">: Index of the mesh</param>
    /// <param name="tex">: Pointer to the texture</param>
    UNDEFINED_ENGINE void SetTexture(int index, std::shared_ptr<Texture> tex);
private:
    /// <summary>
    /// Draw the model
    /// </summary>
    /// <param name="TRS">: The TRS Matrix of the object</param>
    UNDEFINED_ENGINE void Draw(const Matrix4x4& TRS);
    /// <summary>
    /// Load a model
    /// </summary>
    /// <param name="path">: Path to the file containg the Model data (e.g : obj)</param>
    UNDEFINED_ENGINE void LoadModel(const std::string& path);
    /// <summary>
    /// Get all the data that the node contains
    /// </summary>
    /// <param name="node">: aiNode from assimp containing the data</param>
    /// <param name="scene"></param>
    UNDEFINED_ENGINE void ProcessNode(aiNode* node, const aiScene* scene);
    /// <summary>
    /// Get all the data attach to the aiMesh of assimp and convert it into our own Mesh class
    /// </summary>
    /// <param name="mesh">: aiMesh from assimp</param>
    /// <returns>Return our own Mesh</returns>
    UNDEFINED_ENGINE Mesh ProcessMesh(aiMesh* mesh);

    /// <summary>
    /// std::vector of a pair composes with a pointer to a Mesh and a pointer to a Material
    /// </summary>
    std::vector<std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>>> mModel;

    /// <summary>
    /// EBO of the model
    /// </summary>
    unsigned int mEBO = 0;
    /// <summary>
    /// VBO of the model
    /// </summary>
    unsigned int mVBO = 0;
    /// <summary>
    /// VAO of the model
    /// </summary>
    unsigned int mVAO = 0;

    /// <summary>
    /// Pointer to our Renderer to simplify the calls from the ServiceLocator
    /// </summary>
    Renderer* mRenderer = nullptr; 

private:
    /// <summary>
    /// ModelRenderer is a friend class from Model
    /// </summary>
    friend class ModelRenderer;

    friend struct refl_impl::metadata::type_info__ <Model>;
};

REFL_AUTO(type(Model, bases<Resource>)
//field(mModel)
);