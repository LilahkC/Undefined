#pragma once

#include <vector>

#include "world/script.h"

class Player : public Script
{
public:
	void FixedUpdate() override;
};

REFL_AUTO(type(Player, bases<Script>)
);