#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "engine_debug/logger.h"

class BodyActivationListener : public JPH::BodyActivationListener
{
public:
	UNDEFINED_ENGINE virtual void OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;

	UNDEFINED_ENGINE virtual void OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;

};