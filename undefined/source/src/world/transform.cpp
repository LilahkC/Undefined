#include "world/transform.h"

#include <cmath>
#include <toolbox/Matrix3x3.h>
#include <toolbox/Quaternion.h>
#include <toolbox/Calc.h>


void Transform::UpdateTransform()
{
	if (mParentTransform)
	{
		mWorldTRS = mLocalTRS * mParentTransform->WorldMatrix();
	}
	else
	{
		mWorldTRS = mLocalTRS;
	}

	mPosition = { mWorldTRS[0][3], mWorldTRS[1][3], mWorldTRS[2][3] };
	mRotation = mWorldTRS.ToQuaternion();
	Matrix3x3 worldTrans = Matrix4x4::Transpose(mWorldTRS);
	mScale = Vector3(worldTrans[0].Norm(), worldTrans[1].Norm(), worldTrans[2].Norm());

	mLocalPosition = { mLocalTRS[0][3], mLocalTRS[1][3], mLocalTRS[2][3] };
	mLocalRotation = mLocalTRS.ToQuaternion();
	Matrix3x3 localTrans = Matrix4x4::Transpose(mLocalTRS);
	mLocalScale = Vector3(localTrans[0].Norm(), localTrans[1].Norm(), localTrans[2].Norm());
}

const Matrix4x4& Transform::LocalMatrix()
{
	if (mHasChanged)
	{
		mHasChanged = false;
		mWorldTRS = Matrix4x4::TRS(mPosition, mRotation, mScale);
		if (mParentTransform)
		{
			mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
		}
		else
		{
			mLocalTRS = mWorldTRS;
		}
	}

	return mLocalTRS;
}

void Transform::SetLocalMatrix(const Matrix4x4& matrix)
{
	mLocalTRS = matrix;

	mLocalPosition = mLocalTRS[0][3], mLocalTRS[1][3], mLocalTRS[2][3];
	mLocalRotation = mLocalTRS.ToQuaternion();
	Matrix3x3 trans = Matrix4x4::Transpose(mLocalTRS);
	mLocalScale = Vector3(trans[0].Norm(), trans[1].Norm(), trans[2].Norm());

	if (mParentTransform)
	{
		mWorldTRS = mLocalTRS * mParentTransform->WorldMatrix();
	}
	else
	{
		mWorldTRS = mLocalTRS;
	}

	mPosition = mWorldTRS[0][3], mWorldTRS[1][3], mWorldTRS[2][3];
	mRotation = mWorldTRS.ToQuaternion();
	trans = Matrix4x4::Transpose(mWorldTRS);
	mScale = Vector3(trans[0].Norm(), trans[1].Norm(), trans[2].Norm());
}

const Matrix4x4& Transform::WorldMatrix()
{
	if (mHasChanged)
	{
		mHasChanged = false;
		mWorldTRS = Matrix4x4::TRS(mPosition, mRotation, mScale);
		if (mParentTransform)
		{
			mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
		}
		else
		{
			mLocalTRS = mWorldTRS;
		}
	}

	return mWorldTRS;
}

void Transform::SetWorldMatrix(const Matrix4x4& matrix)
{
	mWorldTRS = matrix;

	mPosition = Vector3(mWorldTRS[0][3], mWorldTRS[1][3], mWorldTRS[2][3]);
	mRotation = mWorldTRS.ToQuaternion();
	Matrix3x3 trans = Matrix4x4::Transpose(mWorldTRS);
	mScale = Vector3(trans[0].Norm(), trans[1].Norm(), trans[2].Norm());

	if (mParentTransform)
	{
		mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
	}
	else
	{
		mLocalTRS = mWorldTRS;
	}

	mLocalPosition = Vector3(mLocalTRS[0][3], mLocalTRS[1][3], mLocalTRS[2][3]);
	mLocalRotation = mLocalTRS.ToQuaternion();
	trans = Matrix4x4::Transpose(mLocalTRS);
	mLocalScale = Vector3(trans[0].Norm(), trans[1].Norm(), trans[2].Norm());
}

Vector3 Transform::GetPosition()
{
	return mPosition;
}

void Transform::SetPosition(Vector3 newPosition)
{
	mWorldTRS = Matrix4x4::TRS(newPosition, GetRotationRad(), GetScale());

	if (mParentTransform)
	{
		mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
	}
	else
	{
		mLocalTRS = mWorldTRS;
	}

	mLocalPosition = { mLocalTRS[0][3], mLocalTRS[1][3], mLocalTRS[2][3] };
	mPosition = { mWorldTRS[0][3], mWorldTRS[1][3], mWorldTRS[2][3] };
}

Vector3 Transform::GetRotation()
{
	return mRotation.ToEuler(true);
}

void Transform::SetRotation(const Vector3& newRotation)
{
	SetRotationRad({ calc::ToRad(newRotation.x), calc::ToRad(newRotation.y), calc::ToRad(newRotation.z) });
}

Vector3 Transform::GetRotationRad()
{
	return mRotation.ToEuler();
}

void Transform::SetRotationRad(Vector3 newRotationRad)
{
	newRotationRad.x = std::fmodf(newRotationRad.x, 2.f * calc::PI);
	newRotationRad.y = std::fmodf(newRotationRad.y, 2.f * calc::PI);
	newRotationRad.z = std::fmodf(newRotationRad.z, 2.f * calc::PI);

	mWorldTRS = Matrix4x4::TRS(GetPosition(), newRotationRad, GetScale());

	if (mParentTransform)
	{
		mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
	}
	else
	{
		mLocalTRS = mWorldTRS;
	}

	mLocalRotation = mLocalTRS.ToQuaternion();

	mRotation = mWorldTRS.ToQuaternion();
}

Quaternion Transform::GetRotationQuat()
{
	return mRotation;
}

void Transform::SetRotationQuat(const Quaternion& newRotationQuat)
{
	 mWorldTRS = Matrix4x4::TRS(GetPosition(), newRotationQuat.ToRotationMatrix(), GetScale());

	 if (mParentTransform)
	 {
		 mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
	 }
	 else
	 {
		 mLocalTRS = mWorldTRS;
	 }

	 mLocalRotation = mLocalTRS.ToQuaternion();

	 mRotation = mWorldTRS.ToQuaternion();
}

Vector3 Transform::GetScale()
{
	return mScale;
}

void Transform::SetScale(Vector3 newScale)
{
	mWorldTRS = Matrix4x4::TRS(GetPosition(), GetRotationRad(), newScale);

	if (mParentTransform)
	{
		mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
	}
	else
	{
		mLocalTRS = mWorldTRS;
	}
	
	Matrix3x3 trans = Matrix4x4::Transpose(mLocalTRS);
	mLocalScale = Vector3(trans[0].Norm(), trans[1].Norm(), trans[2].Norm());

	trans = Matrix4x4::Transpose(mWorldTRS);
	mScale = Vector3(trans[0].Norm(), trans[1].Norm(), trans[2].Norm());
}

Vector3 Transform::GetLocalPosition()
{
	return mLocalPosition;
}

void Transform::SetLocalPosition(Vector3 newLocalPosition)
{
	mLocalTRS = Matrix4x4::TRS(newLocalPosition, mLocalRotation, mLocalScale);

	mLocalPosition = newLocalPosition;

	if (mParentTransform)
	{
		mWorldTRS = mLocalTRS * mParentTransform->WorldMatrix();
	}
	else
	{
		mWorldTRS = mLocalTRS;
	}

	mPosition = { mWorldTRS[0][3], mWorldTRS[1][3], mWorldTRS[2][3] };
}

Vector3 Transform::GetLocalRotation()
{
	return mLocalRotation.ToEuler(true);
}

void Transform::SetLocalRotation(Vector3 newLocalRotation)
{
	newLocalRotation.x = calc::ToRad(newLocalRotation.x);
	newLocalRotation.y = calc::ToRad(newLocalRotation.y);
	newLocalRotation.z = calc::ToRad(newLocalRotation.z);
	SetLocalRotationRad(newLocalRotation);
}

Vector3 Transform::GetLocalRotationRad()
{
	return mLocalRotation.ToEuler();
}

void Transform::SetLocalRotationRad(Vector3 newLocalRotationRad)
{
	newLocalRotationRad.x = std::fmodf(newLocalRotationRad.x, 2.f * calc::PI);
	newLocalRotationRad.y = std::fmodf(newLocalRotationRad.y, 2.f * calc::PI);
	newLocalRotationRad.z = std::fmodf(newLocalRotationRad.z, 2.f * calc::PI);

	Matrix4x4 matRot = Matrix4x4::RotationMatrix3D(newLocalRotationRad);
	mLocalTRS = Matrix4x4::TRS(mLocalPosition, matRot, mLocalScale);
	mLocalRotation = mLocalTRS.ToQuaternion();

	if (mParentTransform)
	{
		mWorldTRS = mLocalTRS * mParentTransform->WorldMatrix();
	}
	else
	{
		mWorldTRS = mLocalTRS;
	}
	
	mRotation = mWorldTRS.ToQuaternion();
}

Quaternion Transform::GetLocalRotationQuat()
{
	return mLocalRotation;
}

void Transform::SetLocalRotationQuat(Quaternion newLocalRotationQuat)
{
	mWorldTRS = Matrix4x4::TRS(GetLocalPosition(), newLocalRotationQuat.ToRotationMatrix(), GetLocalScale());

	if (mParentTransform)
	{
		mLocalTRS = mWorldTRS * Matrix4x4::Inverse(mParentTransform->WorldMatrix());
	}
	else
	{
		mLocalTRS = mWorldTRS;
	}

	mLocalRotation = mLocalTRS.ToQuaternion();

	mRotation = mWorldTRS.ToQuaternion();
}

Vector3 Transform::GetLocalScale()
{
	return mLocalScale;
}

void Transform::SetLocalScale(Vector3 newLocalScale)
{
	mLocalTRS = Matrix4x4::TRS(mLocalPosition, mLocalRotation, newLocalScale);
	mLocalScale = newLocalScale;

	if (mParentTransform)
	{
		mWorldTRS = mLocalTRS * mParentTransform->WorldMatrix();
	}
	else
	{
		mWorldTRS = mLocalTRS;
	}

	Matrix3x3 trans = Matrix4x4::Transpose(mWorldTRS);
	mScale = Vector3(trans[0].Norm(), trans[1].Norm(), trans[2].Norm());
}