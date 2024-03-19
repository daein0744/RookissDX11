#include "pch.h"
#include "Transform.h"
#include "MyMatrix.h"
Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::Init()
{
}

void Transform::Update()
{
}

Vec3 ToEulerAngles(Quaternion q) {
	EulerAngles angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.roll = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.pitch = 2 * std::atan2(sinp, cosp) - M_PI / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.yaw = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdageTransform()
{
	Matrix matScale = MyMatrix::CreateScale(_localScale / 3);
	Matrix matRotX = MyMatrix::CreateRotationX(XMConvertToRadians(_localRotation.x));
	Matrix matRotY = MyMatrix::CreateRotationY(XMConvertToRadians(_localRotation.y));
	Matrix matRotZ = MyMatrix::CreateRotationZ(XMConvertToRadians(_localRotation.z));
	Matrix matTranslation = MyMatrix::CreateTranslation(_localPosition);
	_matLocal = matTranslation * matRotX * matRotY * matRotZ * matScale;

	if (HasParent())
	{
		_matWorld = _matLocal;
	}
	else
	{
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}


}