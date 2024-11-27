#pragma once

#include <AL/al.h>
#include <vector>
#include "resources/audio.h"
#include <memory>

struct ALCbuffer;

class SoundBuffer
{
public:
	static SoundBuffer* Get();
	
	ALuint AddSoundEffect(std::shared_ptr<Audio> sound);
	bool RemoveSoundEffect(const ALuint& buffer);

	static inline std::vector<std::string> audioFilesName;

private:
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> mSoundEffectBuffers;
};