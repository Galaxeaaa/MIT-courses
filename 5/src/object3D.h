#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "boundingbox.h"
#include "hit.h"
#include "material.h"
#include "ray.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Grid;

class Object3D
{
public:
	Object3D()
	{
		m = nullptr;
		bbox = nullptr;
	}
	Object3D(const Material *m) : m(m->clone()) {}
	Object3D(const Material &m) : m(m.clone()) {}
	~Object3D() { delete m; }
	void setMaterial(const Material *mat)
	{
		m = mat->clone();
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint() const = 0;
	BoundingBox *getBoundingBox()
	{
		return bbox;
	}
	void setBoundingBox(BoundingBox *bbx)
	{
		bbox = bbx;
	}
	virtual void insertIntoGrid(Grid *g, Matrix *m)
	{
	}

protected:
	Material *m;
	BoundingBox *bbox;
};

#endif