#pragma once

#include <thread>
#include <unordered_map>

#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "engine_debug/logger.h"

#include "world/collider_contact_listener.h"
#include "world/body_activation_listener.h"
#include "world/broad_phase_layer_interface.h"

#include "utils/flag.h"

class PhysicsSystem
{
	STATIC_CLASS(PhysicsSystem)
public:
	static void Init();

	static void Update();

	static void Terminate();

	static void TraceImplentation(const char* inFMT, ...);

	static JPH::Vec3Arg ToJPH(const Vector3& v);
	static JPH::QuatArg ToJPH(const Quaternion& q);
	static Vector3 FromJPH(const JPH::Vec3& v);
	static Quaternion FromJPH(const JPH::Quat& q);

	static unsigned int CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& scale, bool is_static);
	static unsigned int CreateCapsule(const Vector3& pos, const Quaternion& rot, float height, float radius, bool is_static);

	static bool IsBodyActive(unsigned int bodyId);

	static Vector3 GetBodyPosition(unsigned int bodyId);

	static Quaternion GetBodyRotation(unsigned int bodyId);
	
	static void SetPosition(unsigned int bodyId, const Vector3& position);
	
	static void SetRotation(unsigned int bodyId, const Quaternion& rotation);

	static void SetVelocity(unsigned int bodyId, const Vector3& velocity);

	static void AddForce(unsigned int bodyId, const Vector3& force);
	
	static void AddForce(unsigned int bodyId, const Vector3& force, const Vector3& point);

	static void AddImpulse(uint32_t bodyId, const Vector3& impulse);

	static void AddImpulse(uint32_t bodyId, const Vector3& impulse, const Vector3& point);

	static void AddTorque(uint32_t bodyId, const Vector3& torque);

	static void DestroyBody(unsigned int body_ID);

	static Collider* GetColliderFromID(unsigned int bodyId);

	static void SetBoxShape(uint32_t bodyId, const Vector3& size);

	static void SetCapsuleShape(uint32_t bodyId, float halfSize, float radius);

	static inline JPH::TempAllocatorImpl* TempAllocator;

	// We need a job system that will execute physics jobs on multiple threads.
	static inline JPH::JobSystemThreadPool* JobSystem;

	static inline constexpr unsigned int cMaxBodies = 65536;

	static inline constexpr unsigned int cNumBodyMutexes = 0;

	static inline constexpr unsigned int cMaxBodyPairs = 65536;

	static inline constexpr unsigned int cMaxContactConstraints = 10240;

	static inline BroadPhaseLayerInterface BroadphaseLayerInterface;

	static inline JPH::ObjectVsBroadPhaseLayerFilter ObjectVsBroadphaseLayerFilter;

	static inline JPH::ObjectLayerPairFilter ObjectLayerPairFilter;

	static inline JPH::PhysicsSystem* JoltPhysicsSystem;

	static inline BodyActivationListener BodyListener;
	static inline ColliderContactListener ContactListener;

	static inline JPH::BodyInterface* BodyInterface;

	static inline JPH::Body* floor = nullptr;

	static inline std::unordered_map<unsigned int, Collider*> ColliderMap;

};

