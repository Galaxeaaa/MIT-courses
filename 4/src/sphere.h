#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3D.h"
#include <math.h>

#define PI 3.14159265358979323846
#define R2D(angle) ((angle)*PI / 180)

using namespace std;

extern int theta_steps, phi_steps;
extern bool gouraud;

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
				h.set(t1, m->clone(), normal_dir, r);
			}
			return true;
		}
		else if (t2 >= tmin)
		{
			if (t2 < h.getT())
			{
				Vec3f normal_dir = r.pointAtParameter(t2) - center;
				normal_dir.Normalize();
				h.set(t2, m->clone(), normal_dir, r);
			}
			return true;
		}
		else
			return false;
	}

	virtual void paint() const
	{
		float d_theta = R2D(180 / theta_steps);
		float d_phi = R2D(90 / phi_steps);

		glPushMatrix();
		m->glSetMaterial();
		glTranslatef(center.x(), center.y(), center.z());

		glBegin(GL_QUADS);
		for (float iPhi = -R2D(90) + d_phi; iPhi < R2D(90); iPhi += 2 * d_phi)
		{
			for (float iTheta = 0; iTheta < R2D(360); iTheta += 2 * d_theta)
			{
				float cos_theta_p = cos(iTheta + d_theta);
				float cos_theta_m = cos(iTheta - d_theta);
				float sin_theta_p = sin(iTheta + d_theta);
				float sin_theta_m = sin(iTheta - d_theta);
				float cos_phi_p = cos(iPhi + d_phi);
				float cos_phi_m = cos(iPhi - d_phi);
				float sin_phi_p = sin(iPhi + d_phi);
				float sin_phi_m = sin(iPhi - d_phi);

				if (gouraud)
				{
					glNormal3f((radius * cos_theta_p * cos_phi_p), (radius * sin_phi_p), (radius * sin_theta_p * cos_phi_p));
					glVertex3f(radius * cos_theta_p * cos_phi_p, radius * sin_phi_p, radius * sin_theta_p * cos_phi_p);

					glNormal3f((radius * cos_theta_m * cos_phi_p), (radius * sin_phi_p), (radius * sin_theta_m * cos_phi_p));
					glVertex3f(radius * cos_theta_m * cos_phi_p, radius * sin_phi_p, radius * sin_theta_m * cos_phi_p);

					glNormal3f((radius * cos_theta_m * cos_phi_m), (radius * sin_phi_m), (radius * sin_theta_m * cos_phi_m));
					glVertex3f(radius * cos_theta_m * cos_phi_m, radius * sin_phi_m, radius * sin_theta_m * cos_phi_m);

					glNormal3f((radius * cos_theta_p * cos_phi_m), (radius * sin_phi_m), (radius * sin_theta_p * cos_phi_m));
					glVertex3f(radius * cos_theta_p * cos_phi_m, radius * sin_phi_m, radius * sin_theta_p * cos_phi_m);
				}
				else
				{
					// compute appropriate coordinates & normals
					glNormal3f(radius * cos(iTheta) * cos(iPhi), radius * sin(iPhi), radius * sin(iTheta) * cos(iPhi));
					// send gl vertex commands
					glVertex3f(radius * cos_theta_p * cos_phi_p, radius * sin_phi_p, radius * sin_theta_p * cos_phi_p);
					glVertex3f(radius * cos_theta_m * cos_phi_p, radius * sin_phi_p, radius * sin_theta_m * cos_phi_p);
					glVertex3f(radius * cos_theta_m * cos_phi_m, radius * sin_phi_m, radius * sin_theta_m * cos_phi_m);
					glVertex3f(radius * cos_theta_p * cos_phi_m, radius * sin_phi_m, radius * sin_theta_p * cos_phi_m);
				}
			}
		}
		glEnd();
		// glutSolidSphere(radius, 32, 32);
		glPopMatrix();
	}

protected:
	Vec3f center;
	float radius;
};

#endif