#include <AL/al.h>
#include <array>

#include "audio/sound_device.h"
#include "engine_debug/logger.h"

SoundDevice* SoundDevice::Get()
{
	static SoundDevice* soundDevice = new SoundDevice();
	return soundDevice;
}

void SoundDevice::SetOrientation(const Vector3& orientation)
{
	std::array values{orientation, Vector3::UnitY()};

	alListenerfv(AL_ORIENTATION, &values[0].x);
}

void SoundDevice::SetPosition(const Vector3& position)
{
	alListenerfv(AL_POSITION, &position.x);
}

void SoundDevice::SetAttenuation(const int key)
{
	// AL_INVERSE_DISTANCE					0XD001
	// AL_INVERSE_DISTANCE_CLAMPED			0XD002
	// AL_LINEAR_DISTANCE					0XD003
	// AL_LINEAR_DISTANCE_CLAMPED			0XD004
	// AL_EXPONENT_DISTANCE					0XD005
	// AL_EXPONENT_DISTANCE_CLAMPED			0XD006

	if (key < 0XD001 && key > 0XD006)
	{
		Logger::FatalError("Set attenuation key incorrect");
	}

	alListeneri(AL_DISTANCE_MODEL, key);
}

SoundDevice::SoundDevice()
{
	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(mALCDevice, ALC_DEFAULT_ALL_DEVICES_SPECIFIER); //Get device name
	}

	if (!name && alcGetError(nullptr) != ALC_DEVICE_SPECIFIER)
	{
		name = alcGetString(mALCDevice, ALC_DEVICE_SPECIFIER);
	}

	mALCDevice = alcOpenDevice(name); //Get default device (nullptr = default)
	if (!mALCDevice)
	{
		Logger::Warning("Failed to get sound device");
	}

	mALCContext = alcCreateContext(mALCDevice, nullptr); //Create context
	if(!mALCContext)
	{
		Logger::Warning("Failed to set sound context");
	}

	if (!alcMakeContextCurrent(mALCContext)) //Make context current
	{
		Logger::Warning("Failed to make context current");
	}

	Logger::Info("Opened {}", name);
}

SoundDevice::~SoundDevice()
{
	alcDestroyContext(mALCContext);
	alcCloseDevice(mALCDevice);
}
