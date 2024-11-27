#pragma once

#include <string>
#include <toolbox/Matrix4x4.h>

#include "resources/resource.h"
#include "utils/flag.h"

/// <summary>
/// a Class to store the shader
/// </summary>
class Shader : public Resource
{
public:
    /// <summary>
    /// Default Constructor for Shader
    /// </summary>
    UNDEFINED_ENGINE Shader();
    /// <summary>
    /// Constructor for Shader
    /// </summary>
    /// <param name="vertexPath">: Path to the file containing the vertex Shader</param>
    /// <param name="fragmentPath">: Path to the file containing the fragment Shader</param>
    UNDEFINED_ENGINE Shader(const char* vertexPath, const char* fragmentPath);

    // use/activate the shader
    UNDEFINED_ENGINE void Use();

    // unUse/desactivate the shader
    UNDEFINED_ENGINE void UnUse();

    // set Vertex Shader

    /// <summary>
    /// Set the vertex shader
    /// </summary>
    /// <param name="vertex"><: ID of the vertex Shader source/param>
    /// <param name="vShaderCode">: Code of the vertex shader</param>
    /// <returns>Return the shader source ID</returns>
    UNDEFINED_ENGINE unsigned int SetVertexShader(unsigned int vertex, const char* vShaderCode);

    // set Fragment Shader

    /// <summary>
    /// 
    /// </summary>
    /// <param name="fragment">: ID of the fragment Shader source</param>
    /// <param name="fShaderCode">: Code of the fragment shader</param>
    /// <returns></returns>
    UNDEFINED_ENGINE unsigned int SetFragmentShader(unsigned int fragment, const char* fShaderCode);

    /// <summary>
    /// Link the 2 shaders together
    /// </summary>
    /// <param name="vertex">: ID of the vertex Shader source</param>
    /// <param name="fragment">: ID of the fragment Shader source</param>
    UNDEFINED_ENGINE void Link(unsigned int vertex, unsigned int fragment);

    // utility uniform functions
    UNDEFINED_ENGINE void SetBool(const std::string& mName, bool value) const;
    UNDEFINED_ENGINE void SetInt(const std::string& mName, int value) const;
    UNDEFINED_ENGINE void SetFloat(const std::string& mName, float value) const;
    UNDEFINED_ENGINE void SetVec3(const std::string& mName, const Vector3& v) const;
    UNDEFINED_ENGINE void SetMat4(const std::string& mName, const Matrix4x4& m) const;

    /// <summary>
    /// Load a shader by using a vertex and a fragment shader
    /// </summary>
    /// <param name="vertexPath">: Path to the file containing the vertex Shader</param>
    /// <param name="fragmentPath">: Path to the file containing the fragment Shader</param>
    UNDEFINED_ENGINE void Load(const char* vertexPath, const char* fragmentPath);

    /// <summary>
    /// ID of the Shader program
    /// </summary>
    unsigned int ID = 0;
};

