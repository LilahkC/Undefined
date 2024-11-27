#include "world/capsule_collider.h"

#include "wrapper/physics_system.h"

CapsuleCollider::CapsuleCollider()
{
	BodyID = PhysicsSystem::CreateCapsule(mPos, mRot, mHeight, mRadius, false);

	PhysicsSystem::ColliderMap.emplace(BodyID, this);
}

CapsuleCollider::CapsuleCollider(const Vector3& pos, const Quaternion& rot, float height, float radius)
	:mPos(pos), mRot(rot), mHeight(height), mRadius(radius)
{
	BodyID = PhysicsSystem::CreateCapsule(pos, rot, height, radius, false);

	PhysicsSystem::ColliderMap.emplace(BodyID, this);
}

void CapsuleCollider::Update()
{
	PhysicsSystem::SetCapsuleShape(BodyID, mHeight, mRadius);
}

CapsuleCollider::~CapsuleCollider()
{
	if (!JPH::BodyID(BodyID).IsInvalid())
		PhysicsSystem::DestroyBody(BodyID);
}
