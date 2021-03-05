#pragma once

#include "matrix.h"
#include "object3D.h"
#include <queue>

class Transform : public Object3D
{
public:
	Transform(Matrix &m, Object3D *o);

	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint() const;
	virtual BoundingBox *getBoundingBox();

protected:
	Object3D *object;
	Matrix mat;
};