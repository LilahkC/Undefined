#pragma once

#include "world/component.h"

class Collider : public Component
{
public:
	Collider();
	Collider(const Vector3& pos, const Quaternion& rot);
	virtual ~Collider();

	void OnEnable() override;
	void OnDisable() override;

	void Update() override;

	void PreFixedUpdate() override;
	void PostFixedUpdate() override;

	unsigned int BodyID;
	
};

REFL_AUTO(type(Collider, bases<Component>)
);