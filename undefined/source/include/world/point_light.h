#pragma once

#include "world/light.h"

class PointLight : public Light
{
public:
	/// <summary>
	/// Default constructor of PointLight
	/// </summary>
	PointLight();
	/// <summary>
	/// Constructor of PointLight
	/// </summary>
	/// <param name="ambient">: Ambient light</param>
	/// <param name="diffuse">: Diffuse light</param>
	/// <param name="specular">: Specular light</param>
	PointLight(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float constant_attenuation, const float linear_attenuation, const float quadratic_attenuation);
	/// <summary>
	/// Destructor of PointLight
	/// </summary>
	~PointLight();

	/// <summary>
	/// Draw function from Component which is override
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Post Draw function from Component which is override
	/// </summary>
	void PostDraw() override;

	/// <summary>
	/// Get the total number of PointLight
	/// </summary>
	/// <returns></returns>
	int GetNbrOfPointLight() const;

	float ConstantAttenuation = 1.f;
	float LinearAttenuation = 0.09f;
	float QuadraticAttenuation = 0.032f;

private:
	int mID = 0;

private:
	static inline int mGlobalID = 0;
	static inline std::vector<PointLight*> mPointLightsArray;

};

REFL_AUTO(type(PointLight, bases<Light>),
	field(ConstantAttenuation),
	field(LinearAttenuation),
	field(QuadraticAttenuation)
);