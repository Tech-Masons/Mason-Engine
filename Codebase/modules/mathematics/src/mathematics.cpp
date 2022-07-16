#include "Mathematics.h"

#include <math.h>
#include <cassert>

namespace Matrix
{
	/// ADD
	matrix4f Add(matrix4f _matrix0, matrix4f _matrix1)
	{
		matrix4f sum;
		for (int i = 0; i < _4x4_; i++)
		{
			sum.data[i] = _matrix0.data[i] + _matrix1.data[i];
		}
		return sum;
	}

	/// SUBTRACT

	matrix4f Subtract(matrix4f _matrix0, matrix4f _matrix1)
	{
		matrix4f diff;
		for (int i = 0; i < _4x4_; i++)
		{
			diff.data[i] = _matrix0.data[i] + _matrix1.data[i];
		}
		return diff;
	}

	/// MULTIPLY 

	// Multiply _matrix0 and _matrix1
	// "_matrix0 into _matrix1"
	matrix4f Multiply(matrix4f _matrix0, matrix4f _matrix1)
	{
		matrix4f _outMatrix;

		matrix4f _m1 = _matrix0;
		matrix4f _m2 = _matrix1;

		_outMatrix.data[0] = _m1.data[0] * _m2.data[0] + _m1.data[1] * _m2.data[4] + _m1.data[2] * _m2.data[8] + _m1.data[3] * _m2.data[12];
		_outMatrix.data[1] = _m1.data[0] * _m2.data[1] + _m1.data[1] * _m2.data[5] + _m1.data[2] * _m2.data[9] + _m1.data[3] * _m2.data[13];
		_outMatrix.data[2] = _m1.data[0] * _m2.data[2] + _m1.data[1] * _m2.data[6] + _m1.data[2] * _m2.data[10] + _m1.data[3] * _m2.data[14];
		_outMatrix.data[3] = _m1.data[0] * _m2.data[3] + _m1.data[1] * _m2.data[7] + _m1.data[2] * _m2.data[11] + _m1.data[3] * _m2.data[15];

		_outMatrix.data[4] = _m1.data[4] * _m2.data[0] + _m1.data[5] * _m2.data[4] + _m1.data[6] * _m2.data[8] + _m1.data[7] * _m2.data[12];
		_outMatrix.data[5] = _m1.data[4] * _m2.data[1] + _m1.data[5] * _m2.data[5] + _m1.data[6] * _m2.data[9] + _m1.data[7] * _m2.data[13];
		_outMatrix.data[6] = _m1.data[4] * _m2.data[2] + _m1.data[5] * _m2.data[6] + _m1.data[6] * _m2.data[10] + _m1.data[7] * _m2.data[14];
		_outMatrix.data[7] = _m1.data[4] * _m2.data[3] + _m1.data[5] * _m2.data[7] + _m1.data[6] * _m2.data[11] + _m1.data[7] * _m2.data[15];

		_outMatrix.data[8] = _m1.data[8] * _m2.data[0] + _m1.data[9] * _m2.data[4] + _m1.data[10] * _m2.data[8] + _m1.data[11] * _m2.data[12];
		_outMatrix.data[9] = _m1.data[8] * _m2.data[1] + _m1.data[9] * _m2.data[5] + _m1.data[10] * _m2.data[9] + _m1.data[11] * _m2.data[13];
		_outMatrix.data[10] = _m1.data[8] * _m2.data[2] + _m1.data[9] * _m2.data[6] + _m1.data[10] * _m2.data[10] + _m1.data[11] * _m2.data[14];
		_outMatrix.data[11] = _m1.data[8] * _m2.data[3] + _m1.data[9] * _m2.data[7] + _m1.data[10] * _m2.data[11] + _m1.data[11] * _m2.data[15];

		_outMatrix.data[12] = _m1.data[12] * _m2.data[0] + _m1.data[13] * _m2.data[4] + _m1.data[14] * _m2.data[8] + _m1.data[15] * _m2.data[12];
		_outMatrix.data[13] = _m1.data[12] * _m2.data[1] + _m1.data[13] * _m2.data[5] + _m1.data[14] * _m2.data[9] + _m1.data[15] * _m2.data[13];
		_outMatrix.data[14] = _m1.data[12] * _m2.data[2] + _m1.data[13] * _m2.data[6] + _m1.data[14] * _m2.data[10] + _m1.data[15] * _m2.data[14];
		_outMatrix.data[15] = _m1.data[12] * _m2.data[3] + _m1.data[13] * _m2.data[7] + _m1.data[14] * _m2.data[11] + _m1.data[15] * _m2.data[15];

		return _outMatrix;
	}
	
	// Multiply a _matrix by a _scalar
	matrix4f Multiply(matrix4f _matrix, float _scalar)
	{
		matrix4f prod;

		for (int i = 0; i < _4x4_; i++)
		{
			prod.data[i] = _matrix.data[i] * _scalar;
		}

		return prod;
	}
	
	// Multiply Matrix 0 into Space of Matrix 1
	matrix4f MultiplyGlobal(matrix4f _matrix0, matrix4f _matrix1)
	{
		// a -> b
		return Multiply(_matrix0, _matrix1);
	}
	
	// Multiply Matrix 1 into Space of Matrix 0
	matrix4f MultiplyLocal(matrix4f _matrix0, matrix4f _matrix1)
	{
		// b -> a
		return Multiply(_matrix1, _matrix0);
	}

	// Transpos a _matrix
	matrix4f Transpose(matrix4f _matrix)
	{
		matrix4f _mT;

		_mT.data[0] = _matrix.data[0];
		_mT.data[1] = _matrix.data[4];
		_mT.data[2] = _matrix.data[8];
		_mT.data[3] = _matrix.data[12];
		_mT.data[4] = _matrix.data[1];
		_mT.data[5] = _matrix.data[5];
		_mT.data[6] = _matrix.data[9];
		_mT.data[7] = _matrix.data[13];
		_mT.data[8] = _matrix.data[2];
		_mT.data[9] = _matrix.data[6];
		_mT.data[10] = _matrix.data[10];
		_mT.data[11] = _matrix.data[14];
		_mT.data[12] = _matrix.data[3];
		_mT.data[13] = _matrix.data[7];
		_mT.data[14] = _matrix.data[11];
		_mT.data[15] = _matrix.data[15];

		return _mT;
	}

	// Find the Determinant of a _matrix
	float Determinant(matrix4f _matrix)
	{
		float det;

		float a0 = _matrix.data[0] * _matrix.data[5] - _matrix.data[1] * _matrix.data[4];
		float a1 = _matrix.data[0] * _matrix.data[6] - _matrix.data[2] * _matrix.data[4];
		float a2 = _matrix.data[0] * _matrix.data[7] - _matrix.data[3] * _matrix.data[4];
		float a3 = _matrix.data[1] * _matrix.data[6] - _matrix.data[2] * _matrix.data[5];
		float a4 = _matrix.data[1] * _matrix.data[7] - _matrix.data[3] * _matrix.data[5];
		float a5 = _matrix.data[2] * _matrix.data[7] - _matrix.data[3] * _matrix.data[6];

		float b0 = _matrix.data[8] * _matrix.data[13] - _matrix.data[9] * _matrix.data[12];
		float b1 = _matrix.data[8] * _matrix.data[14] - _matrix.data[10] * _matrix.data[12];
		float b2 = _matrix.data[8] * _matrix.data[15] - _matrix.data[11] * _matrix.data[12];
		float b3 = _matrix.data[9] * _matrix.data[14] - _matrix.data[10] * _matrix.data[13];
		float b4 = _matrix.data[9] * _matrix.data[15] - _matrix.data[11] * _matrix.data[13];
		float b5 = _matrix.data[10] * _matrix.data[15] - _matrix.data[11] * _matrix.data[14];

		det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b1;

		return det;
	}

	// INVERSE
	matrix4f Inverse(matrix4f _matrix)
	{
		matrix4f inverse = _matrix;

		float det = 0.0f;

		float a0 = _matrix.data[0] * _matrix.data[5] - _matrix.data[1] * _matrix.data[4];
		float a1 = _matrix.data[0] * _matrix.data[6] - _matrix.data[2] * _matrix.data[4];
		float a2 = _matrix.data[0] * _matrix.data[7] - _matrix.data[3] * _matrix.data[4];
		float a3 = _matrix.data[1] * _matrix.data[6] - _matrix.data[2] * _matrix.data[5];
		float a4 = _matrix.data[1] * _matrix.data[7] - _matrix.data[3] * _matrix.data[5];
		float a5 = _matrix.data[2] * _matrix.data[7] - _matrix.data[3] * _matrix.data[6];

		float b0 = _matrix.data[8] * _matrix.data[13] - _matrix.data[9] * _matrix.data[12];
		float b1 = _matrix.data[8] * _matrix.data[14] - _matrix.data[10] * _matrix.data[12];
		float b2 = _matrix.data[8] * _matrix.data[15] - _matrix.data[11] * _matrix.data[12];
		float b3 = _matrix.data[9] * _matrix.data[14] - _matrix.data[10] * _matrix.data[13];
		float b4 = _matrix.data[9] * _matrix.data[15] - _matrix.data[11] * _matrix.data[13];
		float b5 = _matrix.data[10] * _matrix.data[15] - _matrix.data[11] * _matrix.data[14];

		det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b1;

		if (!(det > 0.0f || det < 0.0f)) {
			//LOG("ERROR!");
		}
		inverse.data[0] = _matrix.data[5] * b5 - _matrix.data[6] * b4 + _matrix.data[7] * b3;
		inverse.data[1] = -_matrix.data[1] * b5 + _matrix.data[2] * b4 - _matrix.data[3] * b3;
		inverse.data[2] = _matrix.data[13] * a5 - _matrix.data[14] * a4 + _matrix.data[15] * a3;
		inverse.data[3] = -_matrix.data[9] * a5 + _matrix.data[10] * a4 - _matrix.data[11] * a3;

		inverse.data[4] = -_matrix.data[4] * b5 + _matrix.data[6] * b2 - _matrix.data[7] * b1;
		inverse.data[5] = _matrix.data[0] * b5 - _matrix.data[2] * b2 + _matrix.data[3] * b1;
		inverse.data[6] = -_matrix.data[12] * a5 + _matrix.data[14] * a2 - _matrix.data[15] * a1;
		inverse.data[7] = _matrix.data[8] * a5 - _matrix.data[10] * a2 + _matrix.data[11] * a1;

		inverse.data[8] = _matrix.data[4] * b4 - _matrix.data[5] * b2 + _matrix.data[7] * b0;
		inverse.data[9] = -_matrix.data[0] * b4 + _matrix.data[1] * b2 - _matrix.data[3] * b0;
		inverse.data[10] = _matrix.data[12] * a4 - _matrix.data[13] * a2 + _matrix.data[15] * a0;
		inverse.data[11] = -_matrix.data[8] * a4 + _matrix.data[9] * a2 - _matrix.data[11] * a0;

		inverse.data[12] = -_matrix.data[4] * b3 + _matrix.data[5] * b1 - _matrix.data[6] * b0;
		inverse.data[13] = _matrix.data[0] * b3 - _matrix.data[1] * b1 + _matrix.data[2] * b0;
		inverse.data[14] = -_matrix.data[12] * a3 + _matrix.data[13] * a1 - _matrix.data[14] * a0;
		inverse.data[15] = _matrix.data[8] * a3 - _matrix.data[9] * a1 + _matrix.data[10] * a0;

		inverse = Multiply(inverse, 1.0f / det);
		return inverse;
	}

	/// Translation:

	// Creates a Translation Matrix
	matrix4f Translation(float _x, float _y, float _z)
	{
		matrix4f trans = Identity;
		trans.data[12] = _x;
		trans.data[13] = _y;
		trans.data[14] = _z;
		return trans;
	}

	// Creates a Translation Matrix
	matrix4f Translation(float3 _position)
	{
		matrix4f trans = Identity;
		trans.data[12] = _position.x;
		trans.data[13] = _position.y;
		trans.data[14] = _position.z;
		return trans;
	}

	// Translate a _matrix by _translation GLOBAL -> m * t
	//matrix4f TranslateGlobal(matrix4f _matrix, float3 _offset) {
	//	return Multiply(_matrix, Translation(_offset));
	//}

	//// Translate a _matrix by _translation LOCAL -> t * m
	//matrix4f TranslateLocal(matrix4f _matrix, float3 _offset) {
	//	return Multiply(Translation(_offset), _matrix);
	//}

	/// Rotation

	// Creates a X-Axis Rotation Matrix
	matrix4f RotationX(float _radian)
	{
		matrix4f rot;

		float c = cos(_radian);
		float s = sin(_radian);

		rot = Identity;
		rot.data[5] = c;
		rot.data[6] = s;
		rot.data[9] = -s;
		rot.data[10] = c;

		return rot;
	}
	// Creates a Y-Axis Rotation Matrix 
	matrix4f RotationY(float _radian)
	{
		matrix4f rot;

		float c = cos(_radian);
		float s = sin(_radian);

		rot = Identity;
		rot.data[0] = c;
		rot.data[2] = -s;
		rot.data[8] = s;
		rot.data[10] = c;

		return rot;
	}
	// Creates a Z-Axis Rotation Matrix
	matrix4f RotationZ(float _radian)
	{
		matrix4f rot;

		float c = cos(_radian);
		float s = sin(_radian);

		rot = Identity;
		rot.data[0] = c;
		rot.data[1] = s;
		rot.data[4] = -s;
		rot.data[5] = c;

		return rot;
	}
	// Rotates a _matrix by a _rotation GLOBAL m * r
	matrix4f RotateGlobal(matrix4f _matrix, matrix4f _rotation)
	{
		// save the translation
		float4 translation = _matrix.row3;
		// apply rotation
		matrix4f result = Multiply(_matrix, _rotation);
		// restore the translation
		result.row3 = translation;

		return result;
	}
	// Rotates a _matrix by a _rotation LOCAL r * m
	matrix4f RotateLocal(matrix4f _matrix, matrix4f _rotation)
	{
		// apply rotation
		matrix4f result = Multiply(_rotation, _matrix);

		return result;
	}
	// Rotate a Matrix along Global X-AXIS by n radians
	matrix4f RotateXGlobal(matrix4f _matrix, float _radian)
	{
		return RotateGlobal(_matrix, RotationX(_radian));
	}
	// Rotate a Matrix along Local X-AXIS by n radians
	matrix4f RotateXLocal(matrix4f _matrix, float _radian)
	{
		return RotateLocal(_matrix, RotationX(_radian));
	}
	// Rotate a Matrix along Global Y-AXIS by n radians
	matrix4f RotateYGlobal(matrix4f _matrix, float _radian)
	{
		return RotateGlobal(_matrix, RotationY(_radian));
	}
	// Rotate a Matrix along Local Y-AXIS by n radians
	matrix4f RotateYLocal(matrix4f _matrix, float _radian)
	{
		return RotateLocal(_matrix, RotationY(_radian));
	}
	// Rotate a Matrix along Global Z-AXIS by n radians
	matrix4f RotateZGlobal(matrix4f _matrix, float _radian)
	{
		return RotateGlobal(_matrix, RotationZ(_radian));
	}
	// Rotate a Matrix along Local Z-AXIS by n radians
	matrix4f RotateZLocal(matrix4f _matrix, float _radian)
	{
		return RotateLocal(_matrix, RotationZ(_radian));
	}
	// Rotate an object Globaly along any axis
	matrix4f RotationRollPitchYaw(float _pitch, float _yaw, float _roll)
	{
		matrix4f final_rotation = Identity;

		matrix4f x = RotateXGlobal(Identity, _pitch);
		matrix4f y = RotateYGlobal(Identity, _yaw);
		matrix4f z = RotateZGlobal(Identity, _roll);

		matrix4f xy;
		xy = Multiply(x, y);
		final_rotation = Multiply(z, xy);

		return final_rotation;
	}
	// Rotate an object Globaly along any axis
	matrix4f RotationRollPitchYaw(float3 _rotation)
	{
		matrix4f final_rotation;

		matrix4f x = RotateXGlobal(Identity, _rotation.x);
		matrix4f y = RotateYGlobal(Identity, _rotation.y);
		matrix4f z = RotateZGlobal(Identity, _rotation.z);

		matrix4f xy;
		xy = Multiply(x, y);
		final_rotation = Multiply(z, xy);

		return final_rotation;
	}

	/// Scale

	// Creates a Scaling Matrix. all axis are scaled independent.
	// NOTE: scales are not preset. 
	matrix4f Scale(float _scaleX, float _scaleY, float _scaleZ)
	{
		matrix4f scale = Identity;
		scale.data[0] = _scaleX;
		scale.data[5] = _scaleY;
		scale.data[10] = _scaleZ;
		return scale;
	}
	// Creates a Scaling Matrix. all axis are scaled independent.
	matrix4f Scale(float3 _scale)
	{
		matrix4f scale = Identity;
		scale.data[0] = _scale.x;
		scale.data[5] = _scale.y;
		scale.data[10] = _scale.z;
		return scale;
	}
	// Creates a Scaling Matrix. all axis are scaled uniformly.
	matrix4f Scale(float _scalar)
	{
		matrix4f scale = Identity;
		scale.data[0] = _scalar;
		scale.data[5] = _scalar;
		scale.data[10] = _scalar;
		return scale;
	}

	// Global Independent Scaling a _matrix by Independent Axis
	matrix4f ScaleGlobal(matrix4f _matrix, float _scaleX, float _scaleY, float _scaleZ)
	{
		return Multiply(_matrix, Scale(_scaleX, _scaleY, _scaleZ));
	}
	// Local Independent Scaling a _matrix by Independent Axis
	matrix4f ScaleLocal(matrix4f _matrix, float _scaleX, float _scaleY, float _scaleZ)
	{
		return Multiply(Scale(_scaleX, _scaleY, _scaleZ), _matrix);
	}

	// Global Independent Scaling a _matrix by float3
	matrix4f ScaleGlobal(matrix4f _matrix, float3 _scale)
	{
		return Multiply(_matrix, Scale(_scale));
	}
	// Local Independent Scaling a _matrix by float3
	matrix4f ScaleLocal(matrix4f _matrix, float3 _scale)
	{
		return Multiply(Scale(_scale), _matrix);
	}

	// Uniform Global Scaling a _matrix by a _scalar
	matrix4f ScaleGlobal(matrix4f _matrix, float _scalar)
	{
		return Multiply(_matrix, Scale(_scalar));
	}
	// Uniform Local Scaling a _matrix by a _scalar
	matrix4f ScaleLocal(matrix4f _matrix, float _scalar)
	{
		return Multiply(Scale(_scalar), _matrix);
	}

	/// PROJECTIONS

	matrix4f ProjectionLH(float _fov, float _aspect_ratio, float _near_z, float _far_z)
	{
		matrix4f _outMatrix = {};

		float yScale = 1.0f / std::tan(_fov / 2.0f);
		float z = _far_z / (_far_z - _near_z);

		_outMatrix = {};
		_outMatrix.row0.data[0] = yScale / _aspect_ratio;
		_outMatrix.row1.data[1] = yScale;
		_outMatrix.row2.data[2] = z;
		_outMatrix.row2.data[3] = 1.0f;
		_outMatrix.row3.data[2] = -_near_z * z;

		return _outMatrix;
	}
	
	matrix4f LookAtLH(float3 _eye, float3 _at, float3 _up)
	{
		matrix4f _outMatrix = Identity;
		float3 temp;
		float3 camDir;	  //zaxis
		float3 camRight;	  //xaxis
		float3 camUp;		  //yaxis
		float magnitudeX;
		float magnitudeZ;

		camDir.x = _at.x - _eye.x;
		camDir.y = _at.y - _eye.y;
		camDir.z = _at.z - _eye.z;

		magnitudeZ = sqrt((camDir.x * camDir.x) + (camDir.y * camDir.y) + (camDir.z * camDir.z));

		camDir.x /= magnitudeZ;
		camDir.y /= magnitudeZ;
		camDir.z /= magnitudeZ;


		camRight.x = (_up.y * camDir.z) - (_up.z * camDir.y);
		camRight.y = (_up.z * camDir.x) - (_up.x * camDir.z);
		camRight.z = (_up.x * camDir.y) - (_up.y * camDir.x);

		magnitudeX = sqrt((camRight.x * camRight.x) + (camRight.y * camRight.y) + (camRight.z * camRight.z));

		camRight.x /= magnitudeX;
		camRight.y /= magnitudeX;
		camRight.z /= magnitudeX;
		

		camUp.x = (camDir.y * camRight.z) - (camDir.z * camRight.y);
		camUp.y = (camDir.z * camRight.x) - (camDir.x * camRight.z);
		camUp.z = (camDir.x * camRight.y) - (camDir.y * camRight.x);

		temp.x = camRight.x * _eye.x + camRight.y * _eye.y + camRight.z * _eye.z;
		temp.y = camUp.x * _eye.x + camUp.y * _eye.y + camUp.z * _eye.z;
		temp.z = camDir.x * _eye.x + camDir.y * _eye.y + camDir.z * _eye.z;

		_outMatrix.data[0] = camRight.x;
		_outMatrix.data[4] = camRight.y;
		_outMatrix.data[8] = camRight.z;
		_outMatrix.data[12] = -temp.x;

		_outMatrix.data[1] = camUp.x;
		_outMatrix.data[5] = camUp.y;
		_outMatrix.data[9] = camUp.z;
		_outMatrix.data[13] = -temp.y;

		_outMatrix.data[2] = camDir.x;
		_outMatrix.data[6] = camDir.y;
		_outMatrix.data[10] = camDir.z;
		_outMatrix.data[14] = -temp.z;

		return _outMatrix;
	}
}

namespace Math {
	// d = sqrt((x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2)

	float Distance(float3 a, float3 b)
	{
		return std::sqrtf(std::powf(b.x - a.x, 2.0f) + std::powf(b.y - a.y, 2.0f) + std::powf(b.z - a.z, 2.0f));
	}

	float Magnitude(float3 f)
	{
		return std::sqrtf(std::powf(f.x, 2.0f) + std::powf(f.y, 2.0f) + std::powf(f.z, 2.0f));
	}

	float MagnitudeSquared(float3 f)
	{
		return std::powf(f.x, 2.0f) + std::powf(f.y, 2.0f) + std::powf(f.z, 2.0f);
	}

	float3 Normalize(float3 f)
	{
		float3 n = { 0 };
		float mag = Magnitude(f);
		n.x = f.x / mag;
		n.y = f.y / mag;
		n.z = f.z / mag;
		return n;
	}

	float Dot(float3 vecA, float3 vecB)
	{
		// calcualte the length of the two vectors
		float a = Magnitude(vecA);
		float b = Magnitude(vecB);

		float theta = 0.0f;
		float dot = 0.0f;
		// are the vectors parrallel?
		if (a * b == 1.0f)
		{
			theta = 0.0f;
			dot = cos(to_radian(theta));
		}
		// no? 
		// then are the vectors perpendictular?
		else if (a * b == 0.0f)
		{
			theta = 90.0f;
			dot =  cos(to_radian(theta));
		}
		// no? 
		// then are the vectors parrallel going opposite ways?
		else if (a * b == -1.0f)
		{
			theta = 180.0f;
			dot = cos(to_radian(theta));
		}
		// else solve normally
		else
		{
			dot = vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
		}

		return dot;
	}

	float3 Cross(float3 a, float3 b)
	{
		float3 cross{0};
		cross.x = a.y * b.z - a.z * b.y;
		cross.y = a.z * b.x - a.x * b.z;
		cross.z = a.x * b.y - a.y * b.x;
		return cross;
	}

	matrix4f LookAt(float3 position, float3 target, float3 up)
	{
		matrix4f look_at = Matrix::Identity;
		
		// calculate the new Z vector
		float3 forward = target - position;
		float3 right = Cross(forward, up);
		
		look_at.row0 = { right.x, right.y, right.z, 0.0f };
		look_at.row1 = { up.x, up.y, up.z, 0.0f };
		look_at.row2 = { forward.x, forward.y, forward.z, 0.0f };
		look_at.row3 = { position.x, position.y, position.z, 0.0f};


		return look_at;
	}

	matrix4f TurnTo(matrix4f target, float3 destination, float speed, float deltaTime)
	{
		matrix4f turn_to = Matrix::Identity;
		// W a position on the plane / the position of the target
		float3 W = target.row3.xyz();

		// find the vector between the two positions
		float3 V = destination - W;
		
		// the current right vector of the target
		float3 X = Normalize(target.row0.xyz());
		
		// the current up vector of the target
		float3 Y = target.row1.xyz();

		// how much we need to rotate along the y-axis
		float VdotX = Math::Dot(V, X);

		// how much we need to rotate along the x-axis
		float VdotY = Math::Dot(V, Y);

		float _yaw = VdotX * speed * deltaTime;

		float _pitch = VdotY * speed * deltaTime;



		matrix4f rot = Matrix::RotationRollPitchYaw({_yaw, _pitch, 0.0f});

		turn_to = Matrix::Multiply(target, rot);
		
		return turn_to;
	}
}