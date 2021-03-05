#pragma once

#include "matrix.h"
#include "object3D.h"
#include <queue>

int max_depth = 0, cur_depth = 0;

queue<Matrix> q;

class Transform : public Object3D
{
public:
    Transform(Matrix &m, Object3D *o) : object(o), mat(m) {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin)
    {
        cur_depth++;
        max_depth = max_depth < cur_depth ? cur_depth : max_depth;
        Vec3f Ro = r.getOrigin();
        Vec3f Rd = r.getDirection();
        Matrix cp = mat;
        cp.Inverse();
        cp.Transform(Ro);
        cp.TransformDirection(Rd);
        // Rd.Normalize();
        cp.Transpose();
        Ray r_transformed(Ro, Rd);
        float prev_t = h.getT();
        bool isintersect = object->intersect(r_transformed, h, tmin);
        if (isintersect && h.getT() != prev_t) // if t updated and this is not leaf
        {
            if (cur_depth == max_depth)
            {
                while (!q.empty())
                    q.pop();
            }
            q.emplace(cp);
        }

        if (cur_depth == 1 && isintersect)
        {
            Vec3f normal = h.getNormal();
            while (!q.empty())
            {
                Matrix mm = q.front();
                q.pop();
                mm.TransformDirection(normal);
                normal.Normalize();
            }
            h.set(h.getT(), h.getMaterial(), normal, r);
            max_depth = 0;
        }

        // cp.TransformDirection(normal);

        cur_depth--;

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