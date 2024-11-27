#include "wrapper/physics_system.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"

#include <cstdarg>
#include <toolbox/Vector3.h>
#include <toolbox/Quaternion.h>

#include "wrapper/time.h"

void PhysicsSystem::Init()
{
	JPH::RegisterDefaultAllocator();

	JPH::Trace = TraceImplentation;

	JPH::Factory::sInstance = new JPH::Factory();

	JPH::RegisterTypes();

	TempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
	JobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
	JoltPhysicsSystem = new JPH::PhysicsSystem();

	JoltPhysicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, BroadphaseLayerInterface, ObjectVsBroadphaseLayerFilter, ObjectLayerPairFilter);

	JoltPhysicsSystem->SetBodyActivationListener(&BodyListener);
	JoltPhysicsSystem->SetContactListener(&ContactListener);

	BodyInterface = &JoltPhysicsSystem->GetBodyInterface();

	JoltPhysicsSystem->SetGravity(ToJPH(Vector3(0, -9.81f, 0)));

	JoltPhysicsSystem->OptimizeBroadPhase();
}

void PhysicsSystem::Update()
{
	JoltPhysicsSystem->Update(Time::FixedDeltaTime, 1, TempAllocator, JobSystem);

	ContactListener.CallOnColliderEnter();
	ContactListener.CallOnColliderStay();
	ContactListener.CallOnColliderExit();
}

void PhysicsSystem::Terminate()
{
	delete JoltPhysicsSystem;
	delete JobSystem;
	delete TempAllocator;

	// Unregisters all types with the factory and cleans up the default material
	JPH::UnregisterTypes();

	// Destroy the factory
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void PhysicsSystem::TraceImplentation(const char* inFMT, ...)
{
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	// Print to the TTY
	std::cout << buffer << std::endl;
}

JPH::Vec3Arg PhysicsSystem::ToJPH(const Vector3& v)
{
	return JPH::RVec3Arg(v.x, v.y, v.z);
}

JPH::QuatArg PhysicsSystem::ToJPH(const Quaternion& q)
{
	return JPH::QuatArg(q.x, q.y, q.z, q.w);
}

Vector3 PhysicsSystem::FromJPH(const JPH::Vec3& v)
{
	return Vector3(v.GetX(), v.GetY(), v.GetZ());
}

Quaternion PhysicsSystem::FromJPH(const JPH::Quat& q)
{
	return Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW());
}

unsigned int PhysicsSystem::CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& scale, bool is_static)
{
	JPH::BodyCreationSettings settings(new JPH::BoxShape(ToJPH(scale/2)), ToJPH(pos), ToJPH(rot), is_static == true ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic, Layers::MOVING);

	//settings.mAllowSleeping = false;
	return BodyInterface->CreateAndAddBody(settings, JPH::EActivation::Activate).GetIndexAndSequenceNumber();
}

unsigned int PhysicsSystem::CreateCapsule(const Vector3& pos, const Quaternion& rot, float height, float radius, bool is_static)
{
	
	JPH::BodyCreationSettings settings(new JPH::CapsuleShape(height / 2, radius), ToJPH(pos), ToJPH(rot), is_static == true ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic, Layers::MOVING);

	//const JPH::CapsuleShapeSettings capsuleSettings(height, radius);
	//const JPH::ShapeSettings::ShapeResult result = capsuleSettings.Create();

	//if (!result.IsValid())
	//{
		//Logger::Error("Couldn't create the capsule");
		//return JPH::BodyID::cInvalidBodyID;
	//}

	//JPH::BodyCreationSettings settings(result.Get(), ToJPH(pos), ToJPH(rot), JPH::EMotionType::Dynamic, Layers::MOVING);

	return BodyInterface->CreateAndAddBody(settings, JPH::EActivation::Activate).GetIndexAndSequenceNumber();
}

bool PhysicsSystem::IsBodyActive(unsigned int bodyId)
{
	return BodyInterface->IsActive(JPH::BodyID(bodyId));
}

Vector3 PhysicsSystem::GetBodyPosition(unsigned int bodyId)
{
	if (!IsBodyActive(bodyId))
	{
		// Logger::Warning("Trying to get the position of an inactive body : {}", bodyId);
		return Vector3();
	}

	const JPH::RVec3 position = BodyInterface->GetCenterOfMassPosition((JPH::BodyID)bodyId);

	return FromJPH(position);
}

Quaternion PhysicsSystem::GetBodyRotation(unsigned int bodyId)
{
	if (!IsBodyActive(bodyId))
	{
		// Logger::Warning("Trying to get the rotation of an inactive body : {}", bodyId);
		return Quaternion();
	}

	const JPH::Quat rotation = BodyInterface->GetRotation((JPH::BodyID)bodyId);

	return FromJPH(rotation);
}

void PhysicsSystem::SetPosition(unsigned int bodyId, const Vector3& position)
{
	BodyInterface->SetPosition((JPH::BodyID)bodyId, JPH::RVec3Arg(position.x, position.y, position.z), JPH::EActivation::DontActivate);
}

void PhysicsSystem::SetRotation(unsigned int bodyId, const Quaternion& rotation)
{
	BodyInterface->SetRotation((JPH::BodyID)bodyId, JPH::QuatArg(rotation.x, rotation.y, rotation.z, rotation.w), JPH::EActivation::DontActivate);
}

void PhysicsSystem::SetVelocity(unsigned int bodyId, const Vector3& velocity)
{
	BodyInterface->SetLinearVelocity((JPH::BodyID)bodyId, ToJPH(velocity));
}

void PhysicsSystem::AddForce(unsigned int bodyId, const Vector3& force)
{
	const JPH::BodyLockWrite lock(JoltPhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

	if (!lock.Succeeded())
		return;

	JPH::Body& body = lock.GetBody();

	body.AddForce(ToJPH(force) / body.GetMotionProperties()->GetInverseMass());
}

void PhysicsSystem::AddForce(unsigned int bodyId, const Vector3& force, const Vector3& point)
{
	const JPH::BodyLockWrite lock(JoltPhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

	if (!lock.Succeeded())
		return;

	JPH::Body& body = lock.GetBody();

	body.AddForce(ToJPH(force) / body.GetMotionProperties()->GetInverseMass(), ToJPH(point));
}

void PhysicsSystem::AddImpulse(uint32_t bodyId, const Vector3& impulse)
{
	const JPH::BodyLockWrite lock(JoltPhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

	if (!lock.Succeeded())
		return;

	JPH::Body& body = lock.GetBody();

	body.AddImpulse(ToJPH(impulse) / body.GetMotionProperties()->GetInverseMass());
}

void PhysicsSystem::AddImpulse(uint32_t bodyId, const Vector3& impulse, const Vector3& point)
{
	const JPH::BodyLockWrite lock(JoltPhysicsSystem->GetBodyLockInterface(), JPH::BodyID(bodyId));

	if (!lock.Succeeded())
		return;

	JPH::Body& body = lock.GetBody();

	body.AddImpulse(ToJPH(impulse) / body.GetMotionProperties()->GetInverseMass(), ToJPH(point));
}

void PhysicsSystem::AddTorque(uint32_t bodyId, const Vector3& torque)
{
	BodyInterface->AddTorque((JPH::BodyID)bodyId, ToJPH(torque));
}

void PhysicsSystem::DestroyBody(unsigned int body_ID)
{
	if (ColliderMap.find(body_ID) == ColliderMap.end())
	{
		Logger::Error("body {} wasn't destroyed properly", body_ID);
		return;
	}
	Logger::Info("body {} was destroyed", body_ID);

	BodyInterface->RemoveBody(JPH::BodyID(body_ID));
	BodyInterface->DestroyBody(JPH::BodyID(body_ID));
	ColliderMap.erase(ColliderMap.find(body_ID));
}

Collider* PhysicsSystem::GetColliderFromID(unsigned int body_ID)
{
	const auto&& it = ColliderMap.find(body_ID);

	if (it == ColliderMap.end())
		return nullptr;

	return it->second;
}

void PhysicsSystem::SetBoxShape(uint32_t bodyId, const Vector3& size)
{
	BodyInterface->SetShape(JPH::BodyID(bodyId), new JPH::BoxShape(ToJPH(size)), false, JPH::EActivation::Activate);
}

void PhysicsSystem::SetCapsuleShape(uint32_t bodyId, float halfSize, float radius)
{
	BodyInterface->SetShape(JPH::BodyID(bodyId), new JPH::CapsuleShape(halfSize, radius), false, JPH::EActivation::Activate);
}
