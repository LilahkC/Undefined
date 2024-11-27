#include "audio/sound_source.h"
#include "engine_debug/logger.h"
#include "audio/sound_buffer.h"

SoundSource::SoundSource()
{
}

SoundSource::~SoundSource()
{
	for (auto&& p : mSoundEffectSource)
	{
		alDeleteSources(1, &p);
	}
}

void SoundSource::Play(ALuint source, const ALuint buffer)
{
	alSourcei(source, AL_BUFFER, (ALint)buffer);
	alGetSourcei(source, AL_SOURCE_STATE, &mState);
	if (mState != AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		alSourcePlay(source);

		std::string name = SoundBuffer::audioFilesName[buffer - 1];
		std::string newName = name.substr(name.find_last_of("/") + 1);
		Logger::Info("Playing {}", newName);
	}
}

void SoundSource::Pause(ALuint source, const ALuint buffer)
{
	alGetSourcei(source, AL_SOURCE_STATE, &mState);
	if (mState == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		alSourcePause(source);

		std::string name = SoundBuffer::audioFilesName[buffer - 1];
		std::string newName = name.substr(name.find_last_of("/") + 1);
		Logger::Info("Paused sound {}", newName);
	}
}

void SoundSource::Resume(ALuint source, const ALuint buffer)
{
	alGetSourcei(source, AL_SOURCE_STATE, &mState);
	if (mState == AL_PAUSED && alGetError() == AL_NO_ERROR)
	{
		alSourcePlay(source);
		std::string name = SoundBuffer::audioFilesName[buffer - 1];
		std::string newName = name.substr(name.find_last_of("/") + 1);
		Logger::Info("Resumed {}", newName);
	}
}

void SoundSource::Stop(ALuint source, const ALuint buffer)
{
	alGetSourcei(source, AL_SOURCE_STATE, &mState);
	if (mState != AL_STOPPED && alGetError() == AL_NO_ERROR)
	{
		alSourceStop(source);
		std::string name = SoundBuffer::audioFilesName[buffer - 1];
		std::string newName = name.substr(name.find_last_of("/") + 1);
		Logger::Info("Stopped playing {}", newName);
	}
}

void SoundSource::Restart(ALuint source, const ALuint buffer)
{
	alGetSourcei(source, AL_SOURCE_STATE, &mState);
	if (mState != AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		alSourceRewind(source);
		alSourcePlay(source);
		std::string name = SoundBuffer::audioFilesName[buffer - 1];
		std::string newName = name.substr(name.find_last_of("/") + 1);
		Logger::Info("Restarted {}", newName);
	}
}

void SoundSource::SetPosition(ALuint source, const Vector3& position)
{
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);
}

void SoundSource::SetVelocity(ALuint source, const Vector3& velocity)
{
	alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void SoundSource::SetVolume(ALuint source, const float volume)
{
	alSourcef(source, AL_GAIN, volume);
}

void SoundSource::SetSpeed(ALuint source, const float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
}

void SoundSource::SetLoop(ALuint source, const bool shouldLoop)
{
	alSourcei(source, AL_LOOPING, shouldLoop);
}

ALuint SoundSource::CreateSource()
{
	ALuint source;
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, mPitch);
	alSourcef(source, AL_GAIN, mGain);
	alSource3f(source, AL_POSITION, mPostition.x, mPostition.y, mPostition.z);
	alSource3f(source, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
	alSourcei(source, AL_LOOPING, mLoop);
	mSoundEffectSource.emplace_back(source);
	return source;
}
