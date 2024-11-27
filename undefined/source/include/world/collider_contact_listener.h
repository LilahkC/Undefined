#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "world/object.h"
#include "world/collider.h"
#include "world/script.h"

#include "engine_debug/logger.h"

class ColliderContactListener : public JPH::ContactListener
{
public:
	// See: ContactListener
	UNDEFINED_ENGINE virtual JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;

	UNDEFINED_ENGINE virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;

	UNDEFINED_ENGINE virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;

	UNDEFINED_ENGINE virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;

public:
	static void CallOnColliderEnter();
	static void CallOnColliderStay();
	static void CallOnColliderExit();
	
private:
	static inline std::vector<std::pair<Script*, const JPH::Body*>> mOnCollisionEnterScripts;
	static inline std::vector<std::pair<Script*, const JPH::Body*>> mOnCollisionStayScripts;
	static inline std::vector<Script*> mOnCollisionExitScripts;

};