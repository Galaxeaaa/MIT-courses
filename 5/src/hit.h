#ifndef _HIT_H
#define _HIT_H

#include "ray.h"
#include "vectors.h"

class Material;

class Hit
{

public:
	// CONSTRUCTOR & DESTRUCTOR
	Hit();
	Hit(float _t, Material *m, Vec3f n, const Ray &ray);
	Hit(const Hit &h);
	~Hit();

	// ACCESSORS
	float getT() const;
	Material *getMaterial() const;
	Vec3f getNormal() const;
	Vec3f getIntersectionPoint() const;

	// MODIFIER
	void set(float _t, Material *m, Vec3f &n, const Ray &ray);

private:
	// REPRESENTATION
	float t;
	Material *material;
	Vec3f normal;
	Vec3f intersectionPoint;
};

inline ostream &operator<<(ostream &os, const Hit &h);

#endif