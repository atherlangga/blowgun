#include "matrix.h"

#include <stdexcept>
#include <iostream>

#include <cmath>
#include <cstring>

#define PI 3.1415926535897932384626433832795f

using namespace blowgun;

// Utility
namespace
{
    static const u32 kColumnCount = 4;

    static u32 Index(u32 row, u32 column)
    {
        return (kColumnCount * row) + column;
    }
}

Matrix::Matrix(const std::vector<float> values)
    : values_(values)
{
}

const float *
Matrix::values() const
{
    return &values_[Index(0,0)];
}

const Matrix
Matrix::Multiply(const Matrix & other) const
{
    std::vector<float> result_values(16);

    for (u32 i = 0; i < kColumnCount; ++i)
    {
        for (u32 j = 0; j < kColumnCount; ++j)
        {
            result_values[Index(i,j)] = 0.0f;
            for (u32 k = 0; k < kColumnCount; ++k)
            {
                result_values[Index(i,j)] +=
                    values_[Index(i,k)] * other.values_[Index(k,j)];
            }
        }
    }

    return Matrix(result_values);
}

const Matrix
Matrix::Scale (float factor_x, float factor_y, float factor_z) const
{
    std::vector<float> result_values(values_);

    result_values[Index(0,0)] *= factor_x;
    result_values[Index(0,1)] *= factor_x;
    result_values[Index(0,2)] *= factor_x;

    result_values[Index(1,0)] *= factor_y;
    result_values[Index(1,1)] *= factor_y;
    result_values[Index(1,2)] *= factor_y;

    result_values[Index(2,0)] *= factor_z;
    result_values[Index(2,1)] *= factor_z;
    result_values[Index(2,2)] *= factor_z;

    return Matrix(result_values);
}

const Matrix
Matrix::Translate(float factor_x, float factor_y, float factor_z) const
{
    std::vector<float> result_values(values_);

    result_values[Index(3,0)] += (
        result_values[Index(0,0)] * factor_x +
        result_values[Index(1,0)] * factor_y +
        result_values[Index(2,0)] * factor_z
        );

    result_values[Index(3,1)] += (
        result_values[Index(0,1)] * factor_x +
        result_values[Index(1,1)] * factor_y +
        result_values[Index(2,1)] * factor_z
        );

    result_values[Index(3,2)] += (
        result_values[Index(0,2)] * factor_x +
        result_values[Index(1,2)] * factor_y +
        result_values[Index(2,2)] * factor_z
        );

    result_values[Index(3,3)] += (
        result_values[Index(0,3)] * factor_x +
        result_values[Index(1,3)] * factor_y +
        result_values[Index(2,3)] * factor_z
        );

    return Matrix(result_values);
}

const Matrix
Matrix::Rotate(float angle,
    float axis_x, float axis_y, float axis_z) const
{
    float sin_angle, cos_angle;
    float mag = sqrtf(
        axis_x * axis_x +
        axis_y * axis_y +
        axis_z * axis_z
        );

    sin_angle = sinf(angle * PI / 180.0f);
    cos_angle = cosf(angle * PI / 180.0f);
    if (mag > 0.0f)
    {
        float xx, yy, zz, xy, yz, zx, xs, ys, zs;
        float one_minus_cos;
        std::vector<float> rotation_values(16);

        axis_x /= mag;
        axis_y /= mag;
        axis_z /= mag;

        xx = axis_x * axis_x;
        yy = axis_y * axis_y;
        zz = axis_z * axis_z;
        xy = axis_x * axis_y;
        yz = axis_y * axis_z;
        zx = axis_z * axis_x;
        xs = axis_x * sin_angle;
        ys = axis_y * sin_angle;
        zs = axis_z * sin_angle;
        one_minus_cos = 1.0f - cos_angle;

        rotation_values[Index(0,0)] = (one_minus_cos * xx) + cos_angle;
        rotation_values[Index(0,1)] = (one_minus_cos * xy) - zs;
        rotation_values[Index(0,2)] = (one_minus_cos * zx) + ys;
        rotation_values[Index(0,3)] = 0.0f;

        rotation_values[Index(1,0)] = (one_minus_cos * xy) + zs;
        rotation_values[Index(1,1)] = (one_minus_cos * yy) + cos_angle;
        rotation_values[Index(1,2)] = (one_minus_cos * yz) - xs;
        rotation_values[Index(1,3)] = 0.0f;

        rotation_values[Index(2,0)] = (one_minus_cos * zx) - ys;
        rotation_values[Index(2,1)] = (one_minus_cos * yz) + xs;
        rotation_values[Index(2,2)] = (one_minus_cos * zz) + cos_angle;
        rotation_values[Index(2,3)] = 0.0f;

        rotation_values[Index(3,0)] = 0.0f;
        rotation_values[Index(3,1)] = 0.0f;
        rotation_values[Index(3,2)] = 0.0f;
        rotation_values[Index(3,3)] = 1.0f;

        Matrix rotation(rotation_values);
        return rotation.Multiply(*this);
    }

    return *this;
}

const Matrix
Matrix::Frustum(float left, float right, float bottom, float top,
    float near_z, float far_z) const
{
    std::vector<float> frustum_values(16);

    float delta_x = right - left;
    float delta_y = top - bottom;
    float delta_z = far_z - near_z;

    if ( (near_z <= 0.0f) || (far_z <= 0.0f) ||
         (delta_x <= 0.0f) || (delta_y <= 0.0f) || (delta_z <= 0.0f) )
         throw new std::runtime_error("Invalid input parameter.");

    frustum_values[Index(0,0)] = 2.0f * near_z / delta_x;
    frustum_values[Index(0,1)] =
        frustum_values[Index(0,2)] =
            frustum_values[Index(0,3)] = 0.0f;

    frustum_values[Index(1,1)] = 2.0f * near_z / delta_y;
    frustum_values[Index(1,0)] =
        frustum_values[Index(1,2)] =
            frustum_values[Index(1,3)] = 0.0f;

    frustum_values[Index(2,0)] = (right + left) / delta_x;
    frustum_values[Index(2,1)] = (top + bottom) / delta_y;
    frustum_values[Index(2,2)] = -(near_z + far_z) / delta_z;
    frustum_values[Index(2,3)] = -1.0f;

    frustum_values[Index(3,2)] = -2.0f * near_z * far_z / delta_z;
    frustum_values[Index(3,0)] =
        frustum_values[Index(3,1)] =
            frustum_values[Index(3,3)] = 0.0f;

    Matrix frustum(frustum_values);
    return frustum.Multiply(*this);
}

const Matrix
Matrix::Perspective(float fov_y, float aspect,
    float near_z, float far_z) const
{
    float frustum_w, frustum_h;

    frustum_h = tanf(fov_y / 360.0f * PI) * near_z;
    frustum_w = frustum_h * aspect;

    return Frustum(
        -frustum_w, frustum_w, -frustum_h, frustum_h, near_z, far_z
        );
}

void
Matrix::Print()
{
    // TODO: This is very very indeaquate implementation,
    // because the matrix here is actually column-based, while
    // this method prints the matrix as if it is row-based.
    for (u32 i = 0; i < 4; ++i)
    {
        for (u32 j = 0; j < 4; ++j)
        {
            std::cout << values_[Index(i,j)] << " ";
        }
        std::cout << std::endl;
    }
}

Matrix
Matrix::CreateIdentity()
{
    // It would be nicer if MSVC supported initializer-list.
    // Until they do, let's use the cumbersome vector initialization.
    float identity_f[] =
    {
        1.0f, 0, 0, 0,
        0, 1.0f, 0, 0,
        0, 0, 1.0f, 0,
        0, 0, 0, 1.0f
    };
    std::vector<float> identity_values(
        identity_f,
        identity_f + sizeof(identity_f) / sizeof(float)
        );

    return Matrix(identity_values);
}
