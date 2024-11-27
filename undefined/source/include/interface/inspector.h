#pragma once

#include <refl.hpp>
#include <type_traits>

#include "utils/flag.h"
#include "resources/model.h"

class UNDEFINED_ENGINE Inspector
{
	STATIC_CLASS(Inspector)

public:
	/// <summary>
	/// Initialize the Inspector
	/// </summary>
	static void Init();
	/// <summary>
	/// Render the window with th inspector content inside
	/// </summary>
	static void ShowWindow();

private:
    /// <summary>
    /// ModelRenderer is a friend class from Model
    /// </summary>
    static inline Renderer* mRenderer;
};