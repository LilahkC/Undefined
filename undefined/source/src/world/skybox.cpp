#include "world/skybox.h"

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "engine_debug/logger.h"

#include "service_locator.h"

#include "interface/interface.h"

void Skybox::Setup()
{
	mSkyboxShader = ResourceManager::Get<Shader>("skybox_shader");

	mRenderer = ServiceLocator::Get<Renderer>();

	mCubemapTexture = Texture::LoadCubeMap(mFaces);
	
	mRenderer->SetCube(mCubeVBO, mCubeVAO);

	mRenderer->ActiveTexture(GL_TEXTURE0);
	mRenderer->BindTexture(mCubemapTexture, GL_TEXTURE_CUBE_MAP);
	mRenderer->BindBuffers(0, 0, 0);
}

void Skybox::Update(Camera* cam)
{
	mRenderer->UseShader(mSkyboxShader->ID);

	for (int i = 0; i < Interface::EditorViewports.size(); i++)
	{
		mView = Matrix4x4(Matrix3x3(cam->GetView())); // remove translation from the view matrix
		mRenderer->SetUniform(mSkyboxShader->ID, "view", mView);
		mRenderer->SetUniform(mSkyboxShader->ID, "projection", cam->GetProjection());
	}

	mRenderer->UnUseShader();
}

void Skybox::Draw()
{
	mRenderer->UseShader(mSkyboxShader->ID);

	mRenderer->SetDepth(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	mRenderer->BindBuffers(mCubeVAO, 0, 0);
	mRenderer->ActiveTexture(GL_TEXTURE0);
	mRenderer->Draw(GL_TRIANGLES, 0, 36);
	mRenderer->BindBuffers(0, 0, 0);

	mRenderer->SetDepth(GL_LESS); // set depth function back to default

	mRenderer->UnUseShader();
}

void Skybox::ChangeFaces()
{

}
