#pragma once

#include <toolbox/Vector3.h>

#include "world/component.h"

#include "resources/shader.h"

#include "utils/flag.h"

#include <refl.hpp>

#define BASE_AMBIENT Vector3{ 0.4f, 0.4f, 0.4f }
#define BASE_DIFFUSE Vector3{ 0.8f, 0.8f, 0.8f }
#define BASE_SPECULAR Vector3{ 0.5f, 0.5f, 0.5f }

/// <summary>
/// Class for Light that inherit from Component
/// </summary>
class Light : public Component
{
public:
	/// <summary>
	/// Default Constructor of Light
	/// </summary>
	UNDEFINED_ENGINE Light();
	/// <summary>
	/// Constructor of Light
	/// </summary>
	/// <param name="ambient">: Ambient light</param>
	/// <param name="diffuse">: Diffuse light</param>
	/// <param name="specular">: Specular light</param>
	UNDEFINED_ENGINE Light(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular);
	/// <summary>
	/// Destructor of Light
	/// </summary>
	/// <returns></returns>
	UNDEFINED_ENGINE ~Light() = 0;

	/// <summary>
	/// Update function from Component which is override
	/// </summary>
	UNDEFINED_ENGINE virtual void Update();

	/// <summary>
	/// Ambient Light
	/// </summary>
	Vector3 Ambient;
	/// <summary>
	/// Diffuse Light
	/// </summary>
	Vector3 Diffuse;
	/// <summary>
	/// Specualar Light
	/// </summary>
	Vector3 Specular;

protected:
	/// <summary>
	/// Shader use for the lights
	/// </summary>
	std::shared_ptr<Shader> pShader;

private:
	void ValueChanged();
};

REFL_AUTO(type(Light, bases<Component>),
	field(Ambient),
	field(Diffuse),
	field(Specular)
);