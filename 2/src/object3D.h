#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "hit.h"
#include "material.h"
#include "ray.h"

class Object3D
{
public:
    Object3D() { m = new Material(Vec3f()); };
    Object3D(const Vec3f &color) { m = new Material(color); };
    ~Object3D() { delete m; };
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

protected:
    Material *m;
};

#endif