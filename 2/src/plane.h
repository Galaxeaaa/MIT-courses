#pragma once

#include "object3D.h"
#include <math.h>

using namespace std;

class Plane : public Object3D
{
public:
    Plane(Vec3f &n, float d, Material *m) : Object3D(m), normal(n), d(d) {}
    ~Plane() {}
    virtual bool intersect(const Ray &r, Hit &h, float tmin)
    {
        Vec3f Ro = r.getOrigin();
        Vec3f Rd = r.getDirection();
        if (normal.Dot3(Rd) == 0)
            return false;
        float t = (-d - normal.Dot3(Ro)) / (normal.Dot3(Rd));
        if (t >= tmin)
        {
            if (t < h.getT())
                h.set(t, m, normal, r);
            return true;
        }
        else
            return false;
    }

protected:
    Vec3f normal;
    float d;
};
