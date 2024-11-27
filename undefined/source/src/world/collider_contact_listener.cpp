#include "world/collider_contact_listener.h"

#include "wrapper/physics_system.h"

JPH::ValidateResult ColliderContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
{
	Logger::Info("Contact validate callback");

	// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ColliderContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	Logger::Info("A contact was added");

	Script* script = PhysicsSystem::GetColliderFromID(inBody1.GetID().GetIndexAndSequenceNumber())->GameObject->GetComponent<Script>();

	if (script)
	{	
		mOnCollisionEnterScripts.push_back(std::make_pair(script, &inBody2));
	}
}

void ColliderContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	Logger::Info("A contact was persisted");
	Script* script = PhysicsSystem::GetColliderFromID(inBody1.GetID().GetIndexAndSequenceNumber())->GameObject->GetComponent<Script>();

	if (script)
	{
		mOnCollisionStayScripts.push_back(std::make_pair(script, &inBody2));
	}
}

void ColliderContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
	Logger::Info("A contact was removed"); 
	Collider* collider = PhysicsSystem::GetColliderFromID(inSubShapePair.GetBody1ID().GetIndexAndSequenceNumber());
	if (collider)
	{
		Script* script = collider->GameObject->GetComponent<Script>();

		if (script)
		{
			mOnCollisionExitScripts.push_back(script);
		}
	}
	else
	{
		Logger::Error("Collider not found");
	}

}

void ColliderContactListener::CallOnColliderEnter()
{
	for (std::pair<Script*, const JPH::Body*> p : mOnCollisionEnterScripts)
	{
		if (!p.first || p.second)
			return;

		p.first->OnCollisionEnter(p.second);
	}

	mOnCollisionEnterScripts.clear();
}

void ColliderContactListener::CallOnColliderStay()
{
	
	for (std::pair<Script*, const JPH::Body*> p : mOnCollisionStayScripts)
	{
		if (!p.first || p.second)
			return;

		p.first->OnCollisionStay(p.second);
	}

	mOnCollisionStayScripts.clear();
	
}

void ColliderContactListener::CallOnColliderExit()
{
	for (Script* s : mOnCollisionExitScripts)
	{
		if (!s)
			return;

		s->OnCollisionExit();
	}

	mOnCollisionExitScripts.clear();

}
