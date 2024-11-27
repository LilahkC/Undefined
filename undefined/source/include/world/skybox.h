#pragma once

#include <string>
#include <toolbox/Matrix3x3.h>
#include <toolbox/Matrix4x4.h>
#include <array>

#include "resources/texture.h"
#include "resources/resource_manager.h"

#include "utils/flag.h"
#include "world/component.h"
#include "camera/camera.h"
#include "resources/shader.h"

class Renderer;

/// <summary>
/// Skybox Class that inherit from Component
/// </summary>
class Skybox : public Component
{
public:
	/// <summary>
	/// Setup the skybox
	/// </summary>
	UNDEFINED_ENGINE static void Setup();
	/// <summary>
	/// Update the skybox
	/// </summary>
	/// <param name="cam">: Camera viewing the skybox</param>
	UNDEFINED_ENGINE static void Update(Camera* cam);
	/// <summary>
	/// Draw the skybox
	/// </summary>
	UNDEFINED_ENGINE static void Draw();

	UNDEFINED_ENGINE static void ChangeFaces();

private:
	/// <summary>
	/// Camera View without the translation
	/// </summary>
	static inline Matrix4x4 mView;

	/// <summary>
	/// VAO
	/// </summary>
	static inline unsigned int mCubeVAO = 0;
	/// <summary>
	/// VBO
	/// </summary>
	static inline unsigned int mCubeVBO = 0;
	/// <summary>
	/// Cubemap Texture
	/// </summary>
	static inline unsigned int mCubemapTexture = 0;

	/// <summary>
	/// Shader of the skybox
	/// </summary>
	static inline std::shared_ptr<Shader> mSkyboxShader;
	/// <summary>
	/// Pointer to our Renderer to simplify the calls from the ServiceLocator
	/// </summary>
	static inline Renderer* mRenderer;
	
	/// <summary>
	/// Path to the right texture for each face
	/// </summary>
	static inline std::vector<std::string> mFaces
	{
		"../undefined/resource_manager/skybox/right.jpg",
		"../undefined/resource_manager/skybox/left.jpg",
		"../undefined/resource_manager/skybox/top.jpg",
		"../undefined/resource_manager/skybox/bottom.jpg",
		"../undefined/resource_manager/skybox/front.jpg",
		"../undefined/resource_manager/skybox/back.jpg"
	};
};

REFL_AUTO(type(Skybox, bases<Component>)
);