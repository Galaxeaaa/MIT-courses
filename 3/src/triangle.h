#pragma once

#include "object3D.h"
#include <math.h>

using namespace std;

class Triangle : public Object3D
{
public:
	Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) : Object3D(m), a(a), b(b), c(c) {}
	~Triangle() {}

	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		Vec3f Ro = r.getOrigin();
		Vec3f Rd = r.getDirection();
		float beta = det3x3(a.x() - Ro.x(), a.x() - c.x(), Rd.x(),
							a.y() - Ro.y(), a.y() - c.y(), Rd.y(),
							a.z() - Ro.z(), a.z() - c.z(), Rd.z()) /
					 det3x3(a.x() - b.x(), a.x() - c.x(), Rd.x(),
							a.y() - b.y(), a.y() - c.y(), Rd.y(),
							a.z() - b.z(), a.z() - c.z(), Rd.z());
		float gamma = det3x3(a.x() - b.x(), a.x() - Ro.x(), Rd.x(),
							 a.y() - b.y(), a.y() - Ro.y(), Rd.y(),
							 a.z() - b.z(), a.z() - Ro.z(), Rd.z()) /
					  det3x3(a.x() - b.x(), a.x() - c.x(), Rd.x(),
							 a.y() - b.y(), a.y() - c.y(), Rd.y(),
							 a.z() - b.z(), a.z() - c.z(), Rd.z());
		float t = det3x3(a.x() - b.x(), a.x() - c.x(), a.x() - Ro.x(),
						 a.y() - b.y(), a.y() - c.y(), a.y() - Ro.y(),
						 a.z() - b.z(), a.z() - c.z(), a.z() - Ro.z()) /
				  det3x3(a.x() - b.x(), a.x() - c.x(), Rd.x(),
						 a.y() - b.y(), a.y() - c.y(), Rd.y(),
						 a.z() - b.z(), a.z() - c.z(), Rd.z());
		Vec3f normal;
		Vec3f::Cross3(normal, a - b, b - c);
		normal.Normalize();
		if (beta + gamma < 1 && beta > 0 && gamma > 0 && t >= tmin)
		{
			if (t < h.getT())
				h.set(t, m, normal, r);
			return true;
		}
		else
			return false;
	}

	virtual void paint() const
	{
		glPushMatrix();
		glBegin(GL_TRIANGLES);
		glVertex3f(a.x(), a.y(), a.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(c.x(), c.y(), c.z());
		glEnd();
		glPopMatrix();
	}

protected:
	Vec3f a, b, c;
};
