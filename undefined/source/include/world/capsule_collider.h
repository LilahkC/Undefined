#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

#include <refl.hpp>

#include <toolbox/Quaternion.h>

#include "world/collider.h"


class CapsuleCollider : public Collider
{
public:
	CapsuleCollider();
	CapsuleCollider(const Vector3& pos, const Quaternion& rot, float height, float radius);

	void Update();

	virtual ~CapsuleCollider();

private:
	Vector3 mPos;
	Quaternion mRot;

	float mHeight;
	float mRadius;

	friend struct refl_impl::metadata::type_info__ <CapsuleCollider>;
};

REFL_AUTO(type(CapsuleCollider, bases<Collider>),
	field(mPos),
	field(mRot),
	field(mHeight),
	field(mRadius)
);