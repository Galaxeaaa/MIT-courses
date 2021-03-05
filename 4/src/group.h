#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3D.h"

class Group : public Object3D
{
public:
    Group(int n) : n(n) { instances = new Object3D *[n]; }

    int getN() { return n; }

    virtual bool intersect(const Ray &r, Hit &h, float tmin)
    {
        bool is = false;
        for (int i = 0; i < n; i++)
        {
            if (instances[i]->intersect(r, h, tmin))
                is = true;
        }
        return is;
    }

    void addObject(int index, Object3D *obj)
    {
        instances[index] = obj;
    }

    virtual void paint() const
    {
        for (int i = 0; i < n; i++)
        {
            instances[i]->paint();
        }
    }

protected:
    int n;
    Object3D **instances;
};

#endif