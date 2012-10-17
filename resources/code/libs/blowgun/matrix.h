#ifndef BLOWGUN_MATRIX_H_
#define BLOWGUN_MATRIX_H_

#include "types.h"

#include <vector>

namespace blowgun
{

class Matrix
{
private:
	const std::vector<float> values_;

	// Private constructor is used in order to keep the data type
	// for Matrix's value private.
	explicit Matrix(const std::vector<float> values);

private:
	// Disallow assignment.
	Matrix & operator=(const Matrix &);

public:

	/**
	 * Get the raw values of the Matrix.
	 *
	 * Please note that this method will return an array contains
	 * the values of the Matrix. This is done so it can be easily
	 * passed in an OpenGL function, such as `glUniformMatrix4fv`.
	 */
	const float * values() const;

	const Matrix Multiply(const Matrix & other) const;
	const Matrix Scale(float factor_x, float factor_y, float factor_z) const;
	const Matrix Translate(float factor_x, float factor_y, float factor_z) const;
	const Matrix Rotate(float angle, float axis_x, float axis_y, float axis_z) const;
	const Matrix Frustum(float left, float right, float bottom, float top, float near_z, float far_z) const;
	const Matrix Perspective(float fov_y, float aspect, float near_z, float far_z) const;

	void Print();

	/**
	 * Create identity matrix.
	 */
	static Matrix CreateIdentity();
};
	
}

#endif // of BLOWGUN_MATRIX_H_
