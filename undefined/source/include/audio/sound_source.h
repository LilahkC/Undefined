#pragma once

#include <AL/al.h>

#include <unordered_map>
#include <toolbox/Vector3.h>
#include "world/component.h"
#include "utils/utils.h"
#include <refl.hpp>

class SoundSource : public Component
{
public:
	SoundSource();
	~SoundSource();

	ALuint CreateSource();

	void Play(ALuint source, const ALuint buffer);
	void Pause(ALuint source, ALuint buffer);
	void Resume(ALuint source, ALuint buffer);
	void Stop(ALuint source, ALuint buffer);
	void Restart(ALuint source, ALuint buffer);

	void SetPosition(ALuint source, const Vector3& position);
	void SetVelocity(ALuint source, const Vector3& velocity);
	void SetVolume(ALuint source, const float volume);
	void SetSpeed(ALuint source, const float pitch);
	void SetLoop(ALuint source, const bool shouldLoop);

private:
	ALuint mBuffer = 0;
	ALuint mSource = 0;
	ALint mState = 0;

	Vector3 mPostition = Vector3(0.f, 0.f, 0.f);
	Vector3 mVelocity = Vector3(0.f, 0.f, 0.f);

	float mGain = 1.f;
	float mPitch = 1.f;
	
	friend struct refl_impl::metadata::type_info__ <SoundSource>;

	bool mLoop = false;
	/// <summary>
	/// Sound source
	/// </summary>
	std::vector<ALuint> mSoundEffectSource;
};

REFL_AUTO(type(SoundSource, bases<Component>),
	field(mGain),
	field(mPitch),
	field(mLoop)
);