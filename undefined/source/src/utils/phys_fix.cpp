#include "utils/phys_fix.h"

void PhysFix::OnCollisionEnter(const JPH::Body* inBody2)
{
	PhysicsSystem::SetVelocity(inBody2->GetID().GetIndexAndSequenceNumber(), Vector3(0,0,0));
}

void PhysFix::OnCollisionStay(const JPH::Body* inBody2)
{
	PhysicsSystem::SetVelocity(inBody2->GetID().GetIndexAndSequenceNumber(), Vector3(0, 0, 0));
}
