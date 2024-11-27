#pragma once

#include "editor.h"
#include "game.h"

#include "resources/texture.h"
#include "resources/shader.h"

#include "engine_debug/logger.h"

#include "utils/flag.h"

class Window;
class Renderer;
class Object;

class Application
{
public:
	UNDEFINED_ENGINE Application();

	UNDEFINED_ENGINE void Init();
	UNDEFINED_ENGINE void Update();
	UNDEFINED_ENGINE void Clear();

	std::shared_ptr<Shader> BaseShader;

	Logger Log;

private:
	Editor mEditor;
	Game mGame;
	Window* mWindowManager = nullptr;
	Renderer* mRenderer = nullptr;

public:
	UNDEFINED_ENGINE static inline bool IsInGame = false;
};

