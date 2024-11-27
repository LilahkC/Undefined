#include "resources/shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <toolbox/Vector3.h>

#include "service_locator.h"
#include "engine_debug/logger.h"

Shader::Shader()
{
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    Load(vertexPath, fragmentPath);
}

void Shader::Use()
{
    ServiceLocator::Get<Renderer>()->UseShader(ID);
}

void Shader::UnUse()
{
    ServiceLocator::Get<Renderer>()->UnUseShader();
}

unsigned int Shader::SetVertexShader(unsigned int vertex, const char* vShaderCode)
{
    vertex = ServiceLocator::Get<Renderer>()->SetShader(GL_VERTEX_SHADER, vShaderCode);

    return vertex;
}

unsigned int Shader::SetFragmentShader(unsigned int fragment, const char* fShaderCode)
{
    fragment = ServiceLocator::Get<Renderer>()->SetShader(GL_FRAGMENT_SHADER, fShaderCode);

    return fragment;
}

void Shader::Link(unsigned int vertex, unsigned int fragment)
{
    ServiceLocator::Get<Renderer>()->LinkShader(ID, vertex, fragment);
}

void Shader::SetBool(const std::string& mName, bool value) const
{
    ServiceLocator::Get<Renderer>()->SetUniform(ID, mName.c_str(), value);
}
void Shader::SetInt(const std::string& mName, int value) const
{
    ServiceLocator::Get<Renderer>()->SetUniform(ID, mName.c_str(), value);
}
void Shader::SetFloat(const std::string& mName, float value) const
{
    ServiceLocator::Get<Renderer>()->SetUniform(ID, mName.c_str(), value);
}
void Shader::SetVec3(const std::string& mName, const Vector3& v) const
{
    ServiceLocator::Get<Renderer>()->SetUniform(ID, mName.c_str(), v);
}
void Shader::SetMat4(const std::string& mName, const Matrix4x4& m) const
{
    ServiceLocator::Get<Renderer>()->SetUniform(ID, mName.c_str(), m);
}

void Shader::Load(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
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
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        Logger::Error("SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex = 0;
    unsigned int fragment = 0;

    // vertex Shader
    vertex = SetVertexShader(vertex, vShaderCode);

    // fragment Shader
    fragment = SetFragmentShader(fragment, fShaderCode);

    // Link shaders
    Link(vertex, fragment);

    // delete the shaders as they're linked into our program now and no longer necessary
    ServiceLocator::Get<Renderer>()->DeleteShader(vertex);
    ServiceLocator::Get<Renderer>()->DeleteShader(fragment);
}
