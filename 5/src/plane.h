#pragma once

#include "object3D.h"
#include <math.h>

#define BIG 10e4

using namespace std;

class Plane : public Object3D
{
public:
	Plane(Vec3f &n, float d, Material *m);
	~Plane();

	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint() const;

protected:
	Vec3f normal;
	float d;
};
