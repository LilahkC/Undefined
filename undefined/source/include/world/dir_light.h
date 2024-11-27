#pragma once

#include <vector>

#include "world/light.h"

/// <summary>
/// Class for the Directional Light that inherit from Light
/// </summary>
class DirLight : public Light
{
public:
	/// <summary>
	/// Default constructor of DirLight
	/// </summary>
	DirLight();
	/// <summary>
	/// Constructor of DirLight
	/// </summary>
	/// <param name="ambient">: Ambient light</param>
	/// <param name="diffuse">: Diffuse light</param>
	/// <param name="specular">: Specular light</param>
	DirLight(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular);
	/// <summary>
	/// Destructor of DirLight
	/// </summary>
	~DirLight();

	/// <summary>
	/// Draw function from Component which is override
	/// </summary>
	void Draw() override;
	/// <summary>
	/// Get the total number of DirLight
	/// </summary>
	/// <returns></returns>
	int GetNbrOfDirLight() const;

	/// <summary>
	/// Direction of the DirLight
	/// </summary>
	Vector3 Direction;

};

REFL_AUTO(type(DirLight, bases<Light>),
	field(Direction)
);