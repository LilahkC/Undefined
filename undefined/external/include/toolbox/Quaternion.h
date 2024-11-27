#pragma once
#pragma warning (disable : 4201)

#include <vector>

#include "Vector3.h"
#include "flag.h"
#include <format>

class Matrix4x4;

#define SQ(var) ((var) * (var))

class TOOLBOX Quaternion
{
public:
	
	union
	{
		struct
		{
			Vector3 imaginary;
			float real;
		};

		struct
		{
			float x, y, z, w;
		};
	};

	Quaternion();
	/// @brief Constructs a Quaternion with all its components set to 'xyzw'.
	Quaternion(const float xyzw);
	Quaternion(const float x, const float y, const float z, const float w);
	Quaternion(const Vector3 imaginary, const float real);
	Quaternion(const std::initializer_list<float>& values);

	/// @brief Returns the length of the quaternion.
	[[nodiscard]]
	float Norm() const;
	/// @brief Returns the squared length of the quaternion.
	[[nodiscard]]
	float SquaredNorm() const;

	/// @brief Returns the conjugate of the quaternion
	[[nodiscard]]
	Quaternion Conjugate() const;

	/// @brief Normalizes the quaternion.
	/// @return A quaternion with the same direction but a length of one.
	[[nodiscard]]
	Quaternion Normalized() const;

	/// @brief Returns the dot product of this Quaternion& with 'other'.
	[[nodiscard]]
	float Dot(const Quaternion& other) const;

	/// @brief Returns the dot product of this Quaternion& with 'other'.
	[[nodiscard]]
	constexpr float Dot(const Quaternion& other);

	/// <summary>
	///  @brief Inverse the Quaternion
	/// @return the inverse quaternions or 0 if square norm is equal to 0
	/// </summary>
	[[nodiscard]]
	Quaternion Inverse() const;

	/// <summary>
	/// @brief Returns the rotation quaternion corresponding to the axis given
	/// </summary>
	static Quaternion GetRotationQuaternion(const Vector3& axis, float angle, bool degrees = false);
	/// <summary>
	/// @brief Returns the rotation quaternion corresponding to the euler angle given
	/// </summary>
	static Quaternion GetRotationQuaternion(const Vector3& euler, bool degrees = false);

	/// <summary>
	///  @brief Returns the quaternion rotate by the rotation given
	/// </summary>
	static Vector3 Rotate(Quaternion& q, Vector3& axis, float angle);
	/// <summary>
	///  @brief Returns the quaternion rotate by the rotation given
	/// </summary>
	static Vector3 Rotate(Quaternion& q, Quaternion RotQuat);
	/// <summary>
	///  @brief Returns the quaternion rotate by the rotation given
	/// </summary>
	Vector3 Rotate(Vector3& axis, float angle) const;
	/// <summary>
	///  @brief Returns the quaternion rotate by the rotation given
	/// </summary>
	Vector3 Rotate(Quaternion RotQuat) const;


	[[nodiscard]]
	Matrix4x4 ToRotationMatrix() const;
	[[nodiscard]]
	Vector3 ToEuler(bool degrees = false) const;

	explicit operator Vector3();

	static Quaternion SLerp(Quaternion& a, Quaternion& b, float t);
};

[[nodiscard]]
Quaternion operator+(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Quaternion operator-(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Quaternion operator-(const Quaternion& a);
[[nodiscard]]
Quaternion operator*(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Quaternion operator*(const Quaternion& q, const float value);
[[nodiscard]]
Quaternion operator/(const Quaternion& a, const Quaternion& b);
[[nodiscard]]
Quaternion operator/(const Quaternion& q, const float value);

Quaternion& operator+=(Quaternion& a, const Quaternion& b);
Quaternion& operator+=(Quaternion& q, const float value);
Quaternion& operator-=(Quaternion& a, const Quaternion& b);
Quaternion& operator-=(Quaternion& q, const float value);
Quaternion& operator*=(Quaternion& a, const Quaternion& b);
Quaternion& operator*=(Quaternion& q, const float value);
Quaternion& operator/=(Quaternion& a, const Quaternion& b);
Quaternion& operator/=(Quaternion& q, const float value);

bool operator==(const Quaternion& q1, const Quaternion& q2);
bool operator!=(const Quaternion& q1, const Quaternion& q2);

bool operator==(const Quaternion& q, const float value);
bool operator!=(const Quaternion& q, const float value);
bool operator<(const Quaternion& q, const float value);
bool operator>(const Quaternion& q, const float value);
bool operator<=(const Quaternion& q, const float value);
bool operator>=(const Quaternion& q, const float value);

std::ostream& operator<<(std::ostream& out, const Quaternion& q);

template <>
struct std::formatter<Quaternion, char>
{
	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end())
			return it;
		if (*it != '}')
			throw std::format_error("Invalid format args for Quaternion");
		return it;
	}

	template<class FmtContext>
	typename FmtContext::iterator format(const Quaternion& quat, FmtContext& ctx) const
	{
		return std::format_to(ctx.out(), "({}, {}, {}, {})", quat.x, quat.y, quat.z, quat.w);
	}
};