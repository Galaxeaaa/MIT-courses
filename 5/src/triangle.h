#pragma once

#include "object3D.h"
#include <math.h>

class Triangle : public Object3D
{
public:
	Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);
	~Triangle();

	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint() const;

protected:
	Vec3f a, b, c;
};
