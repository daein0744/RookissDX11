#include "pch.h"
#include "MathUtils.h"

bool MathUtils::PointInSphere(const Point3D& point, const Sphere3D& sphere)
{
	float magSq = (point - sphere.position).LengthSquared();
	float radSq = sphere.radius * sphere.radius;

	return magSq < radSq;
}

Point3D MathUtils::ClosestPoint(const Sphere3D& sphere, const Point3D& point)
{
	Vec3 sphereToPointDir = point - sphere.position;
	sphereToPointDir.Normalize();
	return sphere.position + sphereToPointDir * sphere.radius;
}

bool MathUtils::PointInAABB(const Point3D& point, const AABB3D& aabb)
{
	Point3D min = AABB3D::GetMin(aabb);
	Point3D max = AABB3D::GetMax(aabb);

	if (point.x < min.x || point.y < min.y || point.z < min.z)
		return false;

	if (point.x > max.x || point.y > max.y || point.z > max.z)
		return false;

	return true;
}

Point3D MathUtils::ClosestPoint(const AABB3D& aabb, const Point3D& point)
{
	Point3D result = point;
	Point3D minPt = AABB3D::GetMin(aabb);
	Point3D maxPt = AABB3D::GetMax(aabb);

	result.x = max(result.x, minPt.x);
	result.y = max(result.y, minPt.y);
	result.z = max(result.z, minPt.z);

	result.x = min(result.x, maxPt.x);
	result.y = min(result.y, maxPt.y);
	result.z = min(result.z, maxPt.z);

	return Point3D();
}

bool MathUtils::PointInOBB(const Point3D& point, const OBB3D& obb)
{
	Vec3 dir = point - obb.position;
	vector<Vec3> axis;
	axis.push_back(obb.orientation.Right());
	axis.push_back(obb.orientation.Up());
	axis.push_back(obb.orientation.Backward());
	
	vector<float> size;
	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (int32 i = 0; i < 3; i++)
	{
		float distance = dir.Dot(axis[i]);

		if (distance > size[i])
			return false;

		if (distance < -size[i])
			return false;
	}

	return true;
}

Point3D MathUtils::ClosestPoint(const OBB3D& obb, const Point3D& point)
{
	Point3D result = obb.position;
	Vec3 dir = point - obb.position;

	vector<Vec3> axis;
	axis.push_back(obb.orientation.Right());
	axis.push_back(obb.orientation.Up());
	axis.push_back(obb.orientation.Backward());

	vector<float> size;
	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (int32 i = 0; i < 3; i++)
	{
		float distance = dir.Dot(axis[i]);

		if (distance > size[i])
			distance = size[i];

		if (distance < -size[i])
			distance = -size[i];

		result = result + (axis[i] * distance);
	}

	return result;
}

bool MathUtils::PointOnPlane(const Point3D& point, const Plane3D& plane)
{
	float dot = point.Dot(plane.normal);
	return dot - plane.distance == 0;
}

Point3D MathUtils::ClosestPoint(const Plane3D& plane, const Point3D& point)
{
	float dot = point.Dot(plane.normal);
	float distance = dot - plane.distance;
	return point - plane.normal * distance;
}

bool MathUtils::PointOnLine(const Point3D& point, const Line3D& line)
{
	Point3D closet = ClosestPoint(line, point);

	float distanceSq = (point - closet).LengthSquared();
	return distanceSq == 0.f;
}

Point3D MathUtils::ClosestPoint(const Line3D& line, const Point3D& point)
{
	Vec3 lVec = line.end - line.start; // Line Vector
	float t = (point - line.start).Dot(lVec) / lVec.Dot(lVec);
	t = fmaxf(t, 0.0f); // Clamp to 0
	t = fminf(t, 1.0f); // Clamp to 1
	return line.start + lVec * t;
}

bool MathUtils::PointOnRay(const Point3D& point, const Ray3D& ray)
{
	if (point == ray.origin)
		return true;
	
	Vec3 norm = point - ray.origin;
	norm.Normalize();
	float diff = norm.Dot(ray.direction);
	return diff == 1.0f;
}

Point3D MathUtils::ClosestPoint(const Ray3D& ray, const Point3D& point)
{
	float t = (point - ray.origin).Dot(ray.direction);
	t = fmaxf(t, 0.0f);

	return Point3D(ray.origin + ray.direction * t);
}

bool MathUtils::SphereSphere(const Sphere3D& s1, const Sphere3D& s2)
{
	float sum = s1.radius + s2.radius;
	float sqDistance = (s1.position - s2.position).LengthSquared();
	return sqDistance <= sum * sum;

}

bool MathUtils::SphereAABB(const Sphere3D& sphere, const AABB3D& aabb)
{
	//Point3D closestPoint = Clo
	return false;
}
