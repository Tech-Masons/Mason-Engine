#pragma once

#include <ostream>

static constexpr float pi = 3.1415926543294f;

#define TO_RADIAN(degree) degree *  (pi / 180.0f)

#define TO_DEGREE(radian) radian * (180.0f / pi)

#pragma region FLOATS

struct float2 {
	union {
		struct {
			float x;
			float y;
		};
		float data[2];
	};

	float2 operator +=(const float2& f2) {
		this->x += f2.x;
		this->y += f2.y;
		return *this;
	}

};

struct float3 {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float data[3];
	};


	float3 operator *(float f) {
		float3 prod;
		prod.x = this->x * f;
		prod.y = this->y * f;
		prod.z = this->z * f;
		return prod;
	}

	float3 operator +(float3 f3) {
		float3 sum = { 0 };
		sum.x = this->x + f3.x;
		sum.y = this->y + f3.y;
		sum.z = this->z + f3.z;
		return sum;
	}

	

	float3 operator /(float3 f3) {
		this->x /= f3.x;
		this->y /= f3.y;
		this->z /= f3.z;
		return *this;
	}
	float3 operator /(float f) {
		float3 div;

		div.x = this->x / f;
		div.y = this->y / f;
		div.z = this->z / f;
		
		return div;
	}



	float3 operator +=(float3 f3) {
		this->x += f3.x;
		this->y += f3.y;
		this->z += f3.z;
		return *this;
	}

	float3 operator -(float3 f3) {
		
		float3 diff;
		diff.x = this->x - f3.x;
		diff.y = this->y - f3.y;
		diff.z = this->z - f3.z;
		return diff;
	}

	float3 operator -=(float3 f3) {
		this->x -= f3.x;
		this->y -= f3.y;
		this->z -= f3.z;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, float3& f) {
		os << "float3: {" << std::endl
			<< "\tX: " << f.x << std::endl
			<< "\tY: " << f.y << std::endl
			<< "\tZ: " << f.z << std::endl
			<< "}" << std::endl;

		return os;
	}
};

struct float4 {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};

	float3 xyz() {
		return { this->x, this->y, this->z };
	};

	float4& operator=(float3& f3) {
		this->x = f3.x;
		this->y = f3.y;
		this->z = f3.z;
		this->w = 1.0f;
		return *this;
	}

};

#pragma endregion

#pragma region MATRICES

#define _4x4_ 16
struct matrix4f {
	union {
		struct {
			float4 row0;
			float4 row1;
			float4 row2;
			float4 row3;
		};
		float data[_4x4_];
	};
};

namespace Matrix {

	constexpr matrix4f Identity{ {{ {{1,0,0,0}}, {{0,1,0,0}},{{0,0,1,0}},{{0,0,0,1}} }} };
	constexpr matrix4f Zero{ {{ {{0,0,0,0}}, {{0,0,0,0}},{{0,0,0,0}},{{0,0,0,0}} }} };


	/// STANDARD MATRIX OPS

	/// ADD
	matrix4f Add(matrix4f _matrix0, matrix4f _matrix1);

	/// SUBTRACT

	matrix4f Subtract(matrix4f _matrix0, matrix4f _matrix1);

	/// MULTIPLY 

	// Multiply _matrix0 and _matrix1
	// "_matrix0 into _matrix1"
	matrix4f Multiply(matrix4f _matrix0, matrix4f _matrix1);
	// Multiply Matrix 0 into Space of Matrix 1
	matrix4f MultiplyGlobal(matrix4f _matrix0, matrix4f _matrix1);
	// Multiply Matrix 1 into Space of Matrix 0
	matrix4f MultiplyLocal(matrix4f _matrix0, matrix4f _matrix1);

	// Multiply a _matrix by a _scalar
	matrix4f Multiply(matrix4f _matrix, float _scalar);

	// Transpos a _matrix
	matrix4f Transpose(matrix4f _matrix);

	// Find the Determinant of a _matrix
	float Determinant(matrix4f _matrix);

	// INVERSE
	matrix4f Inverse(matrix4f _matrix);

	/// Translation:
	// Creates a Translation Matrix
	matrix4f Translation(float _x, float _y, float _z);

	// Creates a Translation Matrix
	matrix4f Translation(float3 _position);

	/// Rotation
	// Creates a X-Axis Rotation Matrix
	matrix4f RotationX(float _radian);
	// Creates a Y-Axis Rotation Matrix 
	matrix4f RotationY(float _radian);
	// Creates a Z-Axis Rotation Matrix
	matrix4f RotationZ(float _radian);
	// Rotates a _matrix by a _rotation GLOBAL m * r
	matrix4f RotateGlobal(matrix4f _matrix, matrix4f _rotation);
	// Rotates a _matrix by a _rotation LOCAL r * m
	matrix4f RotateLocal(matrix4f _matrix, matrix4f _rotation);
	// Rotate a Matrix along Global X-AXIS by n radians
	matrix4f RotateXGlobal(matrix4f _matrix, float _radian);
	// Rotate a Matrix along Local X-AXIS by n radians
	matrix4f RotateXLocal(matrix4f _matrix, float _radian);
	// Rotate a Matrix along Global Y-AXIS by n radians
	matrix4f RotateYGlobal(matrix4f _matrix, float _radian);
	// Rotate a Matrix along Local Y-AXIS by n radians
	matrix4f RotateYLocal(matrix4f _matrix, float _radian);
	// Rotate a Matrix along Global Z-AXIS by n radians
	matrix4f RotateZGlobal(matrix4f _matrix, float _radian);
	// Rotate a Matrix along Local Z-AXIS by n radians
	matrix4f RotateZLocal(matrix4f _matrix, float _radian);
	// Rotate an object Globaly along any axis
	matrix4f RotationRollPitchYaw(float _pitch, float _yaw, float _roll);
	// Rotate an object Globaly along any axis
	matrix4f RotationRollPitchYaw(float3 _rotation);

	/// Scale

	// Creates a Scaling Matrix. all axis are scaled independent.
	// NOTE: scales are not preset. 
	matrix4f Scale(float _scaleX, float _scaleY, float _scaleZ);
	// Creates a Scaling Matrix. all axis are scaled independent.
	matrix4f Scale(float3 _scale);
	// Creates a Scaling Matrix. all axis are scaled uniformly.
	matrix4f Scale(float _scalar);

	// Global Independent Scaling a _matrix by Independent Axis
	matrix4f ScaleGlobal(matrix4f _matrix, float _scaleX, float _scaleY, float _scaleZ);
	// Local Independent Scaling a _matrix by Independent Axis
	matrix4f ScaleLocal(matrix4f _matrix, float _scaleX, float _scaleY, float _scaleZ);

	// Global Independent Scaling a _matrix by float3
	matrix4f ScaleGlobal(matrix4f _matrix, float3 _scale);
	// Local Independent Scaling a _matrix by float3
	matrix4f ScaleLocal(matrix4f _matrix, float3 _scale);

	// Uniform Global Scaling a _matrix by a _scalar
	matrix4f ScaleGlobal(matrix4f _matrix, float _scalar);
	// Uniform Local Scaling a _matrix by a _scalar
	matrix4f ScaleLocal(matrix4f _matrix, float _scalar);

	/// PROJECTIONS

	matrix4f ProjectionLH(float _fov, float _aspect_ratio, float _near_z, float _far_z);

	matrix4f LookAtLH(float3 _eye, float3 _at, float3 up);
}
#pragma endregion

namespace Math {

	float Distance(float3 a, float3 b);

	float Magnitude(float3 f);
	float MagnitudeSquared(float3 f);

	float3 Normalize(float3 f);

	float Dot(float3 a, float3 b);

	float3 Cross(float3 a, float3 b);

	matrix4f LookAt(float3 position, float3 target, float3 up);
	matrix4f TurnTo(matrix4f target, float3 destination, float speed, float deltaTime);
}