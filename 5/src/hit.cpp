#include "hit.h"

Hit::Hit()
{
	material = NULL;
}
Hit::Hit(float _t, Material *m, Vec3f n, const Ray &ray)
{
	t = _t;
	material = m;
	normal = n;
	intersectionPoint = ray.pointAtParameter(t);
}
Hit::Hit(const Hit &h)
{
	t = h.t;
	material = h.material;
	normal = h.normal;
	intersectionPoint = h.intersectionPoint;
}
Hit::~Hit() {}

// ACCESSORS
float Hit::getT() const
{
	return t;
}
Material *Hit::getMaterial() const
{
	return material;
}
Vec3f Hit::getNormal() const
{
	return normal;
}
Vec3f Hit::getIntersectionPoint() const
{
	return intersectionPoint;
}

// MODIFIER
void Hit::set(float _t, Material *m, Vec3f &n, const Ray &ray)
{
	t = _t;
	material = m;
	normal = n;
	normal.Normalize();
	intersectionPoint = ray.pointAtParameter(t);
}

inline ostream &operator<<(ostream &os, const Hit &h)
{
	os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
	return os;
}