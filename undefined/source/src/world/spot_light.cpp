#include "world/spot_light.h"

SpotLight::SpotLight()
{
}

SpotLight::SpotLight(const Vector3& direction, const Vector3& ambient, const Vector3& diffuse, const Vector3& specular, const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic)
	: Light(ambient, diffuse, specular), Direction(direction), CutOff(cutOff), OuterCutOff(outerCutOff), ConstantAttenuation(constant), LinearAttenuation(linear), QuadraticAttenuation(quadratic)
{
	mSpotLightsArray.push_back(this);
}

SpotLight::~SpotLight()
{
}

void SpotLight::Draw()
{
	mID = mGlobalID;
	pShader->Use();
	pShader->SetVec3("spotLights[" + std::to_string(mID) + "].position", GameTransform->Position);
	pShader->SetVec3("spotLights[" + std::to_string(mID) + "].direction", Direction);
	pShader->SetVec3("spotLights[" + std::to_string(mID) + "].ambient", Ambient);
	pShader->SetVec3("spotLights[" + std::to_string(mID) + "].diffuse", Diffuse);
	pShader->SetVec3("spotLights[" + std::to_string(mID) + "].specular", Specular);
	pShader->SetFloat("spotLights[" + std::to_string(mID) + "].constant", ConstantAttenuation);
	pShader->SetFloat("spotLights[" + std::to_string(mID) + "].linear", LinearAttenuation);
	pShader->SetFloat("spotLights[" + std::to_string(mID) + "].quadratic", QuadraticAttenuation);
	pShader->SetFloat("spotLights[" + std::to_string(mID) + "].cutOff", CutOff);
	pShader->SetFloat("spotLights[" + std::to_string(mID) + "].outerCutOff", OuterCutOff);
	pShader->SetBool("spotLights[" + std::to_string(mID) + "].isUsed", true);
	pShader->UnUse();
	mGlobalID++;
}

void SpotLight::PostDraw()
{
	pShader->Use();
	pShader->SetBool("spotLights[" + std::to_string(mID) + "].isUsed", false);
	mGlobalID = 0;
	pShader->UnUse();
}

int SpotLight::GetNbrOfSpotLight() const
{
	return (int)mSpotLightsArray.size();
}
