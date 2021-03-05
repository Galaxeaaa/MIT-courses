#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3D.h"
#include <math.h>

#define PI 3.14159265358979323846
#define R2D(angle) ((angle)*PI / 180)

class Sphere : public Object3D
{
public:
	Sphere(const Vec3f &c, const float &r, Material *m);
	~Sphere();

	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint() const;

protected:
	Vec3f center;
	float radius;
};

#endif