#include "world/box_collider.h"

#include "wrapper/physics_system.h"

#include "service_locator.h"

BoxCollider::BoxCollider()
{
	BodyID = PhysicsSystem::CreateBox(mPos, mRot, mScale, mIsStatic);

	PhysicsSystem::ColliderMap.emplace(BodyID, this);
}

BoxCollider::BoxCollider(const Vector3& pos, const Quaternion& rot, const Vector3& scale, bool is_static)
	: mPos(pos), mRot(rot), mSize(scale), mIsStatic(is_static)
{
	BodyID = PhysicsSystem::CreateBox(pos, rot, scale, is_static);

	PhysicsSystem::ColliderMap.emplace(BodyID, this);
}

void BoxCollider::Update()
{
	PhysicsSystem::SetBoxShape(BodyID, mSize);
}

BoxCollider::~BoxCollider()
{
	if (!JPH::BodyID(BodyID).IsInvalid())
		PhysicsSystem::DestroyBody(BodyID);
}
