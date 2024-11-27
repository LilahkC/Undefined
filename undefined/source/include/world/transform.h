#pragma once

#include <toolbox/Vector3.h>
#include <toolbox/Matrix4x4.h>
#include <toolbox/Quaternion.h>
#include <refl.hpp>

#include "reflection/attributes.h"

#include "utils/flag.h"

class Transform
{
public:
	void UpdateTransform();

	UNDEFINED_ENGINE const Matrix4x4& LocalMatrix();
	UNDEFINED_ENGINE void SetLocalMatrix(const Matrix4x4& matrix);
	UNDEFINED_ENGINE const Matrix4x4& WorldMatrix();
	UNDEFINED_ENGINE void SetWorldMatrix(const Matrix4x4& matrix);

	__declspec(property(get = GetPosition, put = SetPosition)) Vector3 Position;
	UNDEFINED_ENGINE Vector3 GetPosition();
	UNDEFINED_ENGINE void SetPosition(Vector3 newPosition);

	__declspec(property(get = GetRotation, put = SetRotation)) Vector3 Rotation;
	UNDEFINED_ENGINE Vector3 GetRotation();
	UNDEFINED_ENGINE void SetRotation(const Vector3& newRotation);
	__declspec(property(get = GetRotationRad, put = SetRotationRad)) Vector3 RotationRad;
	UNDEFINED_ENGINE Vector3 GetRotationRad();
	UNDEFINED_ENGINE void SetRotationRad(Vector3 newRotationRad);
	__declspec(property(get = GetRotationQuat, put = SetRotationQuat)) Quaternion RotationQuat;
	UNDEFINED_ENGINE Quaternion GetRotationQuat();
	UNDEFINED_ENGINE void SetRotationQuat(const Quaternion& newRotationQuat);

	__declspec(property(get = GetScale, put = SetScale)) Vector3 Scale;
	UNDEFINED_ENGINE Vector3 GetScale();
	UNDEFINED_ENGINE void SetScale(Vector3 newScale);

	__declspec(property(get = GetLocalPosition, put = SetLocalPosition)) Vector3 LocalPosition;
	UNDEFINED_ENGINE Vector3 GetLocalPosition();
	UNDEFINED_ENGINE void SetLocalPosition(Vector3 newLocalPosition);

	__declspec(property(get = GetLocalRotation, put = SetLocalRotation)) Vector3 LocalRotation;
	UNDEFINED_ENGINE Vector3 GetLocalRotation();
	UNDEFINED_ENGINE void SetLocalRotation(Vector3 newLocalRotation);
	__declspec(property(get = GetLocalRotationRad, put = SetLocalRotationRad)) Vector3 LocalRotationRad;
	UNDEFINED_ENGINE Vector3 GetLocalRotationRad();
	UNDEFINED_ENGINE void SetLocalRotationRad(Vector3 newLocalRotationRad);
	__declspec(property(get = GetLocalRotationQuat, put = SetLocalRotationQuat)) Quaternion LocalRotationQuat;
	UNDEFINED_ENGINE Quaternion GetLocalRotationQuat();
	UNDEFINED_ENGINE void SetLocalRotationQuat(Quaternion newLocalRotationQuat);

	__declspec(property(get = GetLocalScale, put = SetLocalScale)) Vector3 LocalScale;
	UNDEFINED_ENGINE Vector3 GetLocalScale();
	UNDEFINED_ENGINE void SetLocalScale(Vector3 newLocalScale);

private:
	bool mHasChanged;
	Vector3 mPosition;
	Quaternion mRotation;
	Vector3 mScale = { 1, 1, 1 };

	Vector3 mLocalPosition;
	Quaternion mLocalRotation;
	Vector3 mLocalScale = { 1, 1, 1 };

	Matrix4x4 mWorldTRS = Matrix4x4::TRS(Vector3(), Quaternion(), Vector3(1));
	Matrix4x4 mLocalTRS = Matrix4x4::TRS(Vector3(), Quaternion(), Vector3(1));
	friend class Object;
	friend class SceneManager;
	friend struct refl_impl::metadata::type_info__ <Transform>;
	Transform* mParentTransform;
};

REFL_AUTO(type(Transform),
	field(mPosition, NotifyChange(&Transform::mHasChanged), Spacing(ImVec2(0, 10))),
	field(mRotation, NotifyChange(&Transform::mHasChanged)),
	field(mScale, NotifyChange(&Transform::mHasChanged))
)