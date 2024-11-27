#include "world/body_activation_listener.h"

#include <Jolt/Physics/Body/BodyID.h>


void BodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{
	Logger::Info("Body number {} got activated", inBodyID.GetIndexAndSequenceNumber());
}

void BodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{
	Logger::Info("Body number {}  went to sleep", inBodyID.GetIndexAndSequenceNumber());
}