#pragma once

#include <memory>

#include "service_locator.h"

#include "resources/texture.h"

#include "engine_debug/logger.h"

class Framebuffer
{
public:
    Framebuffer();
	~Framebuffer();

    /// <summary>
    /// Rescale the framebuffer
    /// </summary>
    /// <param name="width">: New width of the framebuffer</param>
    /// <param name="height">: New height of the framebuufer</param>
    void RescaleFramebuffer(float width, float height);

	/// <summary>
	/// Width of the window
	/// </summary>
	float Width;
	/// <summary>
	/// Height of the window
	/// </summary>
	float Height;

	/// <summary>
	/// Framebuffer ID
	/// </summary>
	unsigned int FBO_ID;
	/// <summary>
	/// Renderbuffer Id
	/// </summary>
	unsigned int RBO_ID;

	/// <summary>
	/// stdd::vector to store the textures usefull for the framebuffer
	/// </summary>
	std::vector<std::unique_ptr<Texture>> FramebufferTextures;

private:
    /// <summary>
    /// Pointer to our Renderer to simplify the calls from the ServiceLocator
    /// </summary>
    Renderer* mRenderer = nullptr;

public:
    /// <summary>
    /// Create a new 
    /// </summary>
    /// <typeparam name="TextureNumber">: Number of textures in the framebuffer</typeparam>
    /// <param name="width">: Base width of the framebuffer</param>
    /// <param name="height">: Base height of the framebuffer</param>
    /// <returns></returns>
    template <size_t TextureNumber>
    static Framebuffer* Create(float width, float height);
};

#include "framebuffer.inl"