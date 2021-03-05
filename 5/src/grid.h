#ifndef _GRID_H_
#define _GRID_H_

#include "object3D.h"

typedef bool statetype;

class Grid : public Object3D
{
public:
	Grid(BoundingBox *bb, int nx, int ny, int nz)
	{
		bbox = bb;
		state = new statetype[nx * ny * nz];
		memset(state, false, sizeof(statetype));
	}
	~Grid()
	{
		delete[] state;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
	}
	virtual void paint() const
	{
	}

private:
	statetype *state;
};

#endif