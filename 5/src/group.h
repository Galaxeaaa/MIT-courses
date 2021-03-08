#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3D.h"

class Group : public Object3D
{
public:
    Group(int n);
	~Group();
    int getN();
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    void addObject(int index, Object3D *obj);
    virtual void paint() const;
	virtual void insertIntoGrid(Grid *g, Matrix *m);

protected:
    int n;
    Object3D **instances;
};

#endif