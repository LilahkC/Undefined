#include "world/point_light.h"

PointLight::PointLight()
{
}

PointLight::PointLight(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float constant_attenuation, const float linear_attenuation, const float quadratic_attenuation)
	: Light(ambient, diffuse, specular), ConstantAttenuation(constant_attenuation), LinearAttenuation(linear_attenuation), QuadraticAttenuation(quadratic_attenuation)
{
	mPointLightsArray.push_back(this);
}

PointLight::~PointLight()
{
}

void PointLight::Draw()
{
	mID = mGlobalID;
	pShader->Use();
	pShader->SetVec3("pointLights[" + std::to_string(mID) + "].position", GameTransform->Position);
	pShader->SetVec3("pointLights[" + std::to_string(mID) + "].ambient", Ambient);
	pShader->SetVec3("pointLights[" + std::to_string(mID) + "].diffuse", Diffuse);
	pShader->SetVec3("pointLights[" + std::to_string(mID) + "].specular", Specular);
	pShader->SetFloat("pointLights[" + std::to_string(mID) + "].constant", ConstantAttenuation);
	pShader->SetFloat("pointLights[" + std::to_string(mID) + "].linear", LinearAttenuation);
	pShader->SetFloat("pointLights[" + std::to_string(mID) + "].quadratic", QuadraticAttenuation);
	pShader->SetBool("pointLights[" + std::to_string(mID) + "].isUsed", true);
	pShader->UnUse();
	mGlobalID++;
}

void PointLight::PostDraw()
{
	mGlobalID = 0;
}

int PointLight::GetNbrOfPointLight() const
{
	return (int)mPointLightsArray.size();
}
