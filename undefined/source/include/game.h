#pragma once

#include "wrapper/physics_system.h"

#include "world/box_collider.h"
#include "world/capsule_collider.h"

#include "world/scene_manager.h"

#include "world/dir_light.h"
#include "world/point_light.h"
#include "world/spot_light.h"

#include "audio/sound_device.h"
#include "audio/sound_source.h"
#include "resources/audio.h"
#include "audio/sound_buffer.h"

#include "resources/model_renderer.h"
#include "resources/resource_manager.h"

#include "interface/interface.h"

class Game
{
public:
	UNDEFINED_ENGINE Game();

	UNDEFINED_ENGINE void Init();
	UNDEFINED_ENGINE void Update();
	UNDEFINED_ENGINE void Terminate();

	SoundDevice* mSoundDevice = nullptr;

	SoundSource* mSoundSource = nullptr;
	uint32_t source1 = 0;
	uint32_t source2 = 0;
	uint32_t sound1 = 0;
	uint32_t sound2 = 0;

	std::shared_ptr<KeyInput> mKeyInput = nullptr;
};

