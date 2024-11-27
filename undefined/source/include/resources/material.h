#pragma once

#include "resources/resource_manager.h"
#include "resources/texture.h"
#include "resources/shader.h"

#include <refl.hpp>

/// <summary>
/// Material Class
/// </summary>
class Material : public Resource
{
public:
	/// <summary>
	/// Constructor of Material
	/// </summary>
	/// <param name="texture">: Pointer to the texture</param>
	/// <param name="shader">: Pointer to the shader</param>
	Material(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader = ResourceManager::Get<Shader>("base_shader"));
	/// <summary>
	/// Destructor of Material
	/// </summary>
	~Material();

	/// <summary>
	/// Texture of the material
	/// </summary>
	std::shared_ptr<Texture> MatTex;
	/// <summary>
	/// Shader of the material
	/// </summary>
	std::shared_ptr<Shader> MatShader;
};

REFL_AUTO(type(Material, bases<Resource>),
	field(MatTex)
	);
