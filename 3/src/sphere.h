#ifndef _SPHERE_H_
#define _SPHERE_H_

#define _USE_MATH_DEFINES
#define R2D(angle) angle * M_PI / 180

#include "object3D.h"
#include <math.h>

using namespace std;

extern int theta_steps, phi_steps;

class Sphere : public Object3D
{
public:
	Sphere(const Vec3f &c, const float &r, Material *m) : Object3D(m), center(c), radius(r) {}
	~Sphere() {}

	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		float _b2, _4ac;
		Vec3f Rd = r.getDirection();
		// Rd.Normalize();
		Vec3f Ro = r.getOrigin();
		_b2 = 4 * Rd.Dot3(Ro - center) * Rd.Dot3(Ro - center);
		_4ac = 4 * Rd.Dot3(Rd) * ((Ro - center).Dot3(Ro - center) - radius * radius);
		if (_b2 < _4ac)
			return false;
		float t1, t2;
		t1 = (-2 * Rd.Dot3(Ro - center) - sqrt(_b2 - _4ac)) / (2 * Rd.Dot3(Rd));
		t2 = (-2 * Rd.Dot3(Ro - center) + sqrt(_b2 - _4ac)) / (2 * Rd.Dot3(Rd));
		if (t1 >= tmin)
		{
			if (t1 < h.getT())
			{
				Vec3f normal_dir = r.pointAtParameter(t1) - center;
				normal_dir.Normalize();
				h.set(t1, m, normal_dir, r);
			}
			return true;
		}
		else if (t2 >= tmin)
		{
			if (t2 < h.getT())
			{
				Vec3f normal_dir = r.pointAtParameter(t2) - center;
				normal_dir.Normalize();
				h.set(t2, m, normal_dir, r);
			}
			return true;
		}
		else
			return false;
	}

	virtual void paint() const
	{
		float d_theta = 180 / theta_steps;
		float d_phi = 90 / phi_steps;

		glPushMatrix();
		glBegin(GL_QUADS);
		for (int iPhi = -90 + d_phi; iPhi < 90; iPhi += 2 * d_phi)
		{
			for (int iTheta = 0; iTheta < 360; iTheta += 2 * d_theta)
			{
				// compute appropriate coordinates & normals
				glNormal3f(radius * cos(iTheta) * cos(iPhi) - center.x(), radius * sin(iPhi) - center.y(), radius * sin(iTheta) * cos(iPhi) - center.z());
				// send gl vertex commands
				glVertex3f(center.x() + radius * cos(iTheta + d_theta) * cos(iPhi + d_phi), center.y() + radius * sin(iPhi + d_phi), center.z() + radius * sin(iTheta + d_theta) * cos(iPhi + d_phi));
				glVertex3f(center.x() + radius * cos(iTheta - d_theta) * cos(iPhi + d_phi), center.y() + radius * sin(iPhi + d_phi), center.z() + radius * sin(iTheta - d_theta) * cos(iPhi + d_phi));
				glVertex3f(center.x() + radius * cos(iTheta - d_theta) * cos(iPhi - d_phi), center.y() + radius * sin(iPhi - d_phi), center.z() + radius * sin(iTheta - d_theta) * cos(iPhi - d_phi));
				glVertex3f(center.x() + radius * cos(iTheta + d_theta) * cos(iPhi - d_phi), center.y() + radius * sin(iPhi - d_phi), center.z() + radius * sin(iTheta + d_theta) * cos(iPhi - d_phi));
			}
		}
		glEnd();
		glPopMatrix();
	}

protected:
	Vec3f center;
	float radius;
};

#endif