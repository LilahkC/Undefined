#include "wrapper/key_input.h"

#include <algorithm>

KeyInput::KeyInput(const std::vector<int>& keysToMonitor) : mIsEnabled(true)
{
    for (int key : keysToMonitor)
    {
        mKeysMap[key] = false;
    }
}

KeyInput::~KeyInput()
{
}

const bool KeyInput::GetIsKeyDown(int key)
{
    bool result = false;

    if (mIsEnabled)
    {
        std::map<int, bool>::iterator it = mKeysMap.find(key);

        if (it != mKeysMap.end())
        {
            result = mKeysMap[key];
        }
       
    }

    return result;
}

bool KeyInput::GetIsEnabled()
{
    return mIsEnabled;
}

void KeyInput::SetIsEnabled(bool value)
{
    mIsEnabled = value;
}

void KeyInput::SetIsKeyDown(int key, bool isDown)
{
    mKeysMap[key] = isDown;
}


