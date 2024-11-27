#include "resources/material.h"

Material::Material(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader)
	: MatTex(texture), MatShader(shader)
{
}

Material::~Material()
{
}