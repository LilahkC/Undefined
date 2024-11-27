#pragma once

#include "world/component.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <refl.hpp>

class Script : public Component
{
public:

	virtual void Start();

	virtual void Update();

	virtual void FixedUpdate();

	virtual void LateUpdate();

	virtual void OnEnable();

	virtual void OnDisable();

	virtual void OnCollisionEnter(const JPH::Body* inBody2);

	virtual void OnCollisionStay(const JPH::Body* inBody2);

	virtual void OnCollisionExit();

	virtual void OnDestroy();
};

REFL_AUTO(type(Script, bases<Component>)
);