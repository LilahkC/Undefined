#pragma once

#include "world/component.h"
#include "resources/model.h"

#include <refl.hpp>

/// <summary>
/// Class for ModelRenderer that draw the model
/// </summary>
class ModelRenderer : public Component
{
public:
	ModelRenderer();

	/// <summary>
	/// Draw the model
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Model of the Object
	/// </summary>
	std::shared_ptr<Model> ModelObject;
};
 
REFL_AUTO(type(ModelRenderer, bases<Component>),
	field(ModelObject)
);