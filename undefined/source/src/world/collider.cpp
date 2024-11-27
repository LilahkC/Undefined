#include "world/collider.h"

#include "wrapper/physics_system.h"

Collider::Collider()
{
}

Collider::Collider(const Vector3& pos, const Quaternion& rot)
{

}

Collider::~Collider()
{

}

void Collider::OnEnable()
{
	PhysicsSystem::BodyInterface->ActivateBody((JPH::BodyID)BodyID);
}

void Collider::OnDisable()
{
	PhysicsSystem::BodyInterface->DeactivateBody((JPH::BodyID)BodyID);
}

void Collider::Update()
{
}

void Collider::PreFixedUpdate()
{
	if (IsEnable())
	{
		PhysicsSystem::SetPosition(BodyID, GameTransform->GetPosition());
		PhysicsSystem::SetRotation(BodyID, GameTransform->GetRotationQuat());
	}
}

void Collider::PostFixedUpdate()
{
	if (!IsEnable())
		return;

	Vector3 Bodypos = PhysicsSystem::GetBodyPosition(BodyID);
	Quaternion Bodyrot = PhysicsSystem::GetBodyRotation(BodyID);

	Vector3 eulerRot = Bodyrot.ToEuler(true);

	if (isnan<float>(Bodypos.x) || isnan<float>(Bodypos.y) || isnan<float>(Bodypos.z))
		return;

	if (isnan<float>(eulerRot.x) || isnan<float>(eulerRot.y) || isnan<float>(eulerRot.z))
		return;

	if (Bodypos == Vector3())
		return;

	if (Bodyrot == Quaternion())
		return;


	GameTransform->SetPosition(Bodypos);
	GameTransform->SetRotationQuat(Bodyrot);
}
