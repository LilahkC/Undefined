#include "resources/audio.h"

#include <fstream>

#include "engine_debug/logger.h"
#include "audio/sound_buffer.h"

Audio::Audio()
{
}

Audio::Audio(const char* filename)
{
	mName = filename;
	mData = LoadWAV(filename, mChannel, mSampleRate, mBps, mSize);
}

Audio::~Audio()
{
	mData = nullptr;
	mName = "";
}

char* Audio::GetData()
{
	return mData;
}

ALushort Audio::GetChannel()
{
	return mChannel;
}

ALushort Audio::GetBPS()
{
	return mBps;
}

ALint Audio::GetSampleRate()
{
	return mSampleRate;
}

ALint Audio::GetSize()
{
	return mSize;
}

std::string Audio::GetName()
{
	return mName;
}

char* Audio::LoadWAV(const char* fn, ALushort& chan, ALint& samplerate, ALushort& bps, ALint& size)
{
	std::ifstream file(fn, std::ios::in | std::ios::binary | std::ios::ate);
	size_t fileSize = file.tellg();
	file.seekg(0);

	if (!file.good())
	{
		Logger::Warning("File {} could not be open", fn);
	}

	char buffer[4];
	file.read(buffer, 4);

	if (strncmp(buffer, "RIFF", 4) == 0)
	{
		for (size_t i = 12; i < fileSize; i++)
		{
			if (strncmp(buffer, "fmt ", 4) == 0)
			{
				file.seekg(i + 8);
				file.read((char*)&chan, 2); //Number of channels
				file.read((char*)&samplerate, 4); //Sample rate
				Logger::Debug("{}", (int)file.cur);
				file.seekg(i + 20);
				file.read((char*)&bps, 2); //Bits per sample
			}
			if (strncmp(buffer, "data", 4) == 0)
			{
				file.seekg(i + 2);
				file.read((char*)&size, 4); //size
				char* data = new char[size];
				file.read(data, size);
				return data;
			}
			file.read(buffer, 4);
			file.seekg(i);
		}
	}
	return NULL;
}