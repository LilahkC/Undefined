#pragma once

#include "utils/flag.h"

class UNDEFINED_ENGINE MemoryLeak
{
public:
	static void CheckMemoryLeak(bool printResult = false);
	static void EndMemoryLeak();

private:
	static inline bool mIsMemoryLeak;
};