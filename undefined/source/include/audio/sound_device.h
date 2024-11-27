#pragma once
#include <AL/alc.h>
#include <toolbox/Vector3.h>

class SoundDevice
{
public:
	static SoundDevice* Get();
	void SetOrientation(const Vector3& orientation);
	void SetPosition(const Vector3& position);

	/// <summary>
	/// Set attenuation for sound device
	/// </summary>
	/// <param name="key">
	/// AL_INVERSE_DISTANCE				//		
	/// AL_INVERSE_DISTANCE_CLAMPED		//
	/// AL_LINEAR_DISTANCE				//
	/// AL_LINEAR_DISTANCE_CLAMPED		//
	/// AL_EXPONENT_DISTANCE			//
	/// AL_EXPONENT_DISTANCE_CLAMPED
	/// </param>
	void SetAttenuation(const int key);

private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* mALCDevice;
	ALCcontext* mALCContext;
};