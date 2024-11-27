#pragma once

#include <compare>
#include <ostream>
#include <vector>
#include <format>

#include "Vector3.h"
#include "flag.h"

class Vector;
class Matrix;
class Matrix2x2;
class Matrix4x4;

/// @brief The Matrix3x3 class represents a two-dimensional array mainly used for mathematical operations.
class TOOLBOX Matrix3x3
{
public:
    /// @brief Returns the identity matrix.
    ///        The identity matrix is a matrix with its diagonal
    ///        set to one and everything else set to zero.
    static Matrix3x3 Identity();

    /// @brief Creates a matrix with all its values set to this default value.
    Matrix3x3(const float defaultValue = 0.f);
    Matrix3x3(const Vector3& r0, const Vector3& r1, const Vector3& r2);
    Matrix3x3(
        const float r00, const float r01, const float r02,
        const float r10, const float r11, const float r12,
        const float r20, const float r21, const float r22
    );
    /// @brief Copies the content of the given matrix to this one.
    Matrix3x3(const Matrix3x3& matrix);

    /// @brief Returns whether the matrix has everything except its diagonal set to zero.
    [[nodiscard]]
    bool IsDiagonal() const;
    /// @brief Returns whether the matrix is the identity matrix.
    ///        If this returns true, Matrix3x3::Identity() == *this should be true.
    [[nodiscard]]
    bool IsIdentity() const;
    /// @brief Returns wether this matrix has everything set to zero.
    [[nodiscard]]
    bool IsNull() const;
    /// @brief Returns whether the matrix is symmetric by its diagonal elements.
    [[nodiscard]]
    bool IsSymmetric() const;
    /// @brief Returns whether the matrix is symmetric by its diagonal elements
    ///        but one of the sides is the opposite of the other.
    [[nodiscard]]
    bool IsAntisymmetric() const;
    /// @brief Returns the diagonal elements of the matrix.
    [[nodiscard]]
    Vector3 Diagonal() const;
    /// @brief Returns the sum of the diagonal elements of the matrix.
    [[nodiscard]]
    float Trace() const;
    /// @brief Returns a matrix with its data set to the given indices of this one.
    [[nodiscard]]
    Matrix3x3 SubMatrix(const size_t rowIndex, const size_t colIndex, const size_t rows, const size_t cols) const;
    /// @brief Returns the determinant of this matrix.
    [[nodiscard]]
    float Determinant() const;
    /// @brief Sets this matrix to the identity matrix.
    Matrix3x3& LoadIdentity();
    /// @brief Switches the matrix by its diagonal elements.
    Matrix3x3& Transpose();
    /// @brief Computes the inverse of the given matrix using the Gauss-Jordan pivot.
    Matrix3x3& Inverse();
    /// @brief Computes the ortho normal of the given matrix using Gram Schimdt method.
    Matrix3x3& OrthoNormalise();

    /// @brief Switches the given matrix by its diagonal elements.
    [[nodiscard]]
    static Matrix3x3 Transpose(const Matrix3x3& matrix);
    /// @brief Computes the inverse of the given matrix using the Gauss-Jordan pivot.
    [[nodiscard]]
    static Matrix3x3 Inverse(const Matrix3x3& matrix);
    /// @brief Computes the ortho normal of the given matrix using Gram Schimdt method.
    static Matrix3x3 OrthoNormalise(const Matrix3x3& matrix);

    [[nodiscard]]
    constexpr const Vector3& operator[](const size_t row) const;
    [[nodiscard]]
    constexpr Vector3& operator[](const size_t row);

    explicit operator Vector3() const;
    explicit operator Vector() const;
    explicit operator Matrix2x2() const;
    operator Matrix4x4() const;
    operator Matrix() const;

    // Automatically generates all comparison operators
    [[nodiscard]]
    friend auto operator<=>(const Matrix3x3& a, const Matrix3x3& b) = default;

private:
    Vector3 r0, r1, r2;
};

[[nodiscard]]
Matrix3x3 operator-(const Matrix3x3& matrix);
[[nodiscard]]
Matrix3x3 operator+(const Matrix3x3& m1, const Matrix3x3& m2);
[[nodiscard]]
Matrix3x3 operator-(const Matrix3x3& m1, const Matrix3x3& m2);
[[nodiscard]]
Matrix3x3 operator*(const Matrix3x3& m, const float scalar);
[[nodiscard]]
Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2);

Matrix3x3& operator+=(Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3& operator-=(Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3& operator*=(Matrix3x3& m, const float scalar);
Matrix3x3& operator*=(Matrix3x3& m1, const Matrix3x3& m2);

std::ostream& operator<<(std::ostream& out, const Matrix3x3& m);

template <>
struct std::formatter<Matrix3x3, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        if (it == ctx.end())
            return it;
        if (*it != '}')
            throw std::format_error("Invalid format args for Matrix3x3");
        return it;
    }

    template<class FmtContext>
    typename FmtContext::iterator format(const Matrix3x3& mat, FmtContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}\n{}\n{}", mat[0], mat[1], mat[2]);
    }
};