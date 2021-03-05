#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3D.h"

class Group : public Object3D
{
public:
    Group(int n);
    int getN();
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    void addObject(int index, Object3D *obj);
    virtual void paint() const;

protected:
    int n;
    Object3D **instances;
};

#endif