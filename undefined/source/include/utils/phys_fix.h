#pragma once

#include "world/script.h"
#include "wrapper/physics_system.h"

class PhysFix : public Script
{
public:
	void OnCollisionEnter(const JPH::Body* inBody2) override;

	void OnCollisionStay(const JPH::Body* inBody2) override;
};