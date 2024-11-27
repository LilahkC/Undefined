#include "world/dir_light.h"

#include "resources/resource_manager.h"
#include "resources/shader.h"

DirLight::DirLight()
{

}

DirLight::DirLight(const Vector3& ambient, const Vector3& diffuse, const Vector3& specular)
	: Light(ambient, diffuse, specular)
{
}

DirLight::~DirLight()
{
}

void DirLight::Draw()
{
	pShader->Use();
	Direction = GameTransform->Position;
	pShader->SetVec3("dirLights[0].direction", Direction);
	pShader->SetVec3("dirLights[0].ambient", Ambient);
	pShader->SetVec3("dirLights[0].diffuse", Diffuse);
	pShader->SetVec3("dirLights[0].specular", Specular);
	pShader->UnUse();
}
