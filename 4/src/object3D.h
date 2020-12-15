#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "hit.h"
#include "material.h"
#include "ray.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Object3D
{
public:
    Object3D() { m = NULL; }
    Object3D(const Material *m) : m(m->clone()) {}
    Object3D(const Material &m) : m(m.clone()) {}
    ~Object3D() { delete m; }
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() const = 0;

protected:
    Material *m;
};

#endif