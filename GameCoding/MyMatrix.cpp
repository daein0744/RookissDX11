#include "pch.h"
#include "MyMatrix.h"

Matrix MyMatrix::CreateScale(const Vec3& scales)
{
	Matrix matScale;
	matScale._11 = scales.x;
	matScale._22 = scales.y;
	matScale._33 = scales.z;
	return matScale;

}

Matrix MyMatrix::CreateRotationX(float radians)
{
	Matrix matRotX;
	float sin, cos;

	XMScalarSinCos(&sin, &cos, radians);
	matRotX._22 = cos;
	matRotX._23 = sin;
	matRotX._33 = cos;
	matRotX._32 = -sin;

	return matRotX;
}

Matrix MyMatrix::CreateRotationY(float radians)
{
	Matrix matRotY;
	float sin, cos;

	XMScalarSinCos(&sin, &cos, radians);
	matRotY._11 = cos;
	matRotY._13 = sin;
	matRotY._33 = cos;
	matRotY._31 = -sin;

	return matRotY;
}

Matrix MyMatrix::CreateRotationZ(float radians)
{
	Matrix matRotZ;
	float sin, cos;

	XMScalarSinCos(&sin, &cos, radians);
	matRotZ._11 = cos;
	matRotZ._12 = sin;
	matRotZ._22 = cos;
	matRotZ._21 = -sin;

	return matRotZ;
}

Matrix MyMatrix::CreateTranslation(const Vec3 positions)
{
	Matrix matTranslation;
	matTranslation._41 = positions.x;
	matTranslation._42 = positions.y;
	matTranslation._43 = positions.z;

	return matTranslation;
}
