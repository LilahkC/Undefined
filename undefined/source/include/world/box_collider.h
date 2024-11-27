#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include <refl.hpp>

#include <toolbox/Quaternion.h>

#include "world/collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();
	BoxCollider(const Vector3& pos, const Quaternion& rot, const Vector3& scale, bool is_static = false);

	void Update();
	virtual ~BoxCollider();

private:
	Vector3 mPos;
	Quaternion mRot;
	Vector3 mScale;
	Vector3 mSize;
	bool mIsStatic = false;
	
	friend struct refl_impl::metadata::type_info__ <BoxCollider>;
};


REFL_AUTO(type(BoxCollider, bases<Collider>),
	field(mPos),
	field(mRot),
	field(mScale),
	field(mSize),
	field(mIsStatic)
);