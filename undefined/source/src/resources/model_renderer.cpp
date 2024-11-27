#include "resources/model_renderer.h"

#include "resources/model.h"

#include "imgui/imgui.h"

ModelRenderer::ModelRenderer()
{
}

void ModelRenderer::Draw()
{
	if (ModelObject)
	{
		ModelObject->Draw(GameTransform->WorldMatrix());
	}
}
