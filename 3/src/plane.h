#pragma once

#include "object3D.h"
#include <math.h>

#define BIG 10e5

using namespace std;

class Plane : public Object3D
{
public:
    Plane(Vec3f &n, float d, Material *m) : Object3D(m), normal(n), d(d)
    {
        normal.Normalize();
    }
    ~Plane() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin)
    {
        Vec3f Ro = r.getOrigin();
        Vec3f Rd = r.getDirection();
        if (normal.Dot3(Rd) == 0)
            return false;
        float t = (d - normal.Dot3(Ro)) / (normal.Dot3(Rd));
        if (t >= tmin)
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
        Vec3f b1, b2;
        Vec3f::Cross3(b1, normal, Vec3f(1, 0, 0));
        if (b1.Length() == 0)
        {
            Vec3f::Cross3(b1, normal, Vec3f(0, 1, 0));
        }
        Vec3f::Cross3(b2, normal, b1);
        b1.Normalize();
        b2.Normalize();

        glPushMatrix();
        m->glSetMaterial();
        glTranslatef(d * normal.x(), d * normal.y(), d * normal.z());
        glBegin(GL_QUADS);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(BIG * (b1.x() + b2.x()), BIG * (b1.y() + b2.y()), BIG * (b1.z() + b2.z()));
        glVertex3f(BIG * (-b1.x() + b2.x()), BIG * (-b1.y() + b2.y()), BIG * (-b1.z() + b2.z()));
        glVertex3f(BIG * (-b1.x() - b2.x()), BIG * (-b1.y() - b2.y()), BIG * (-b1.z() - b2.z()));
        glVertex3f(BIG * (b1.x() - b2.x()), BIG * (b1.y() - b2.y()), BIG * (b1.z() - b2.z()));
        glEnd();

        glPopMatrix();
    }

protected:
    Vec3f normal;
    float d;
};
