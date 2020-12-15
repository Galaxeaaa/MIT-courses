#pragma once

#include "object3D.h"

class Transform : public Object3D
{
public:
    Transform(Matrix &m, Object3D *o) : object(o), mat(m) {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin)
    {
        Vec3f Ro = r.getOrigin();
        Vec3f Rd = r.getDirection();
        Matrix cp = mat;
        cp.Inverse();
        cp.Transform(Ro);
        cp.TransformDirection(Rd);
        // Rd.Normalize();
        cp.Transpose();
        Ray r_transformed(Ro, Rd);
        bool isintersect = object->intersect(r_transformed, h, tmin);
        Vec3f normal = h.getNormal();
        cp.Transform(normal);
        normal.Normalize();
        h.set(h.getT(), h.getMaterial(), normal, r_transformed);

        return isintersect;
    }

    virtual void paint() const
    {
        glPushMatrix();
        GLfloat *glMatrix = mat.glGet();
        glMultMatrixf(glMatrix);
        delete[] glMatrix;
        object->paint();
        glPopMatrix();
    }

protected:
    Object3D *object;
    Matrix mat;
};