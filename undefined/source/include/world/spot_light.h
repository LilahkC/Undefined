#pragma once

#include "world/light.h"

class SpotLight : public Light
{
public:
	/// <summary>
	/// Default constructor of SpotLight
	/// </summary>
	SpotLight();
	/// <summary>
	/// Constructor of SpotLight
	/// </summary>
	/// <param name="ambient">: Ambient light</param>
	/// <param name="diffuse">: Diffuse light</param>
	/// <param name="specular">: Specular light</param>
	SpotLight(const Vector3& direction, const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic);
	/// <summary>
	/// Destructor of SpotLight
	/// </summary>
	~SpotLight();

	/// <summary>
	/// Draw function from Component which is override
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Post Draw function from Component which is override
	/// </summary>
	void PostDraw() override;

	/// <summary>
	/// Get the total number of SpotLight
	/// </summary>
	/// <returns></returns>
	int GetNbrOfSpotLight() const;

	Vector3 Direction;

	float ConstantAttenuation = 1.f;
	float LinearAttenuation = 0.09f;
	float QuadraticAttenuation = 0.032f;

	float CutOff;
	float OuterCutOff;

private:
	int mID = 0;

private:
	static inline int mGlobalID = 0;
	static inline std::vector<SpotLight*> mSpotLightsArray;

};

REFL_AUTO(type(SpotLight, bases<Light>),
	field(Direction),
	field(ConstantAttenuation),
	field(LinearAttenuation),
	field(QuadraticAttenuation),
	field(CutOff),
	field(OuterCutOff)
);
