#pragma once
#include "SimpleMath.h"

class MyMatrix : Matrix
{
public:
	static Matrix CreateScale(const Vec3& scales);
	static Matrix CreateRotationX(float radians);
	static Matrix CreateRotationY(float radians);
	static Matrix CreateRotationZ(float radians);
	static Matrix CreateTranslation(const Vec3 positions);
};

