#pragma once

#include "resources/texture.h"
#include "resources/model.h"
#include "resources/model_renderer.h"
#include "resources/resource_manager.h"

#include "interface/interface.h"
#include "interface/inspector.h"

class Editor
{
public:
	UNDEFINED_ENGINE Editor();

	UNDEFINED_ENGINE void Init();
	UNDEFINED_ENGINE void Update();
	UNDEFINED_ENGINE void Terminate();
};

