#ifndef _HIT_H
#define _HIT_H

#include "ray.h"
#include "vectors.h"

class Material;

// ====================================================================
// ====================================================================

class Hit
{

public:
	// CONSTRUCTOR & DESTRUCTOR
	Hit() { material = NULL; }
	Hit(float _t, Material *m)
	{
		t = _t;
		material = m;
	}
	Hit(const Hit &h)
	{
		t = h.t;
		material = h.material;
		intersectionPoint = h.intersectionPoint;
	}
	~Hit() {}

	// ACCESSORS
	float getT() const { return t; }
	Material *getMaterial() const { return material; }
	Vec3f getIntersectionPoint() const { return intersectionPoint; }
	Vec3f getNormal() const { return normal; }

	// MODIFIER
	void set(float _t, Material *m, const Ray &ray)
	{
		t = _t;
		material = m;
		intersectionPoint = ray.pointAtParameter(t);
	}
	void set(const Vec3f &norm)
	{
		normal = norm;
	}

private:
	// REPRESENTATION
	float t;
	Material *material;
	Vec3f intersectionPoint;
	Vec3f normal;
};

inline ostream &operator<<(ostream &os, const Hit &h)
{
	os << "Hit <t:" << h.getT() << ">";
	return os;
}
// ====================================================================
// ====================================================================

#endif