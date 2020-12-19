#pragma once

#include "object3D.h"
#include <queue>

queue<Matrix> q;

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
        q.emplace(cp);
        Ray r_transformed(Ro, Rd);
        bool isintersect = object->intersect(r_transformed, h, tmin);
        if (isintersect)
        {
            Vec3f normal = h.getNormal();
            Matrix mm = q.front();
            q.pop();
            mm.TransformDirection(normal);
            // cp.TransformDirection(normal);
            normal.Normalize();
            h.set(h.getT(), h.getMaterial(), normal, r);
        }
        else
        {
            while (!q.empty())
                q.pop();
        }

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