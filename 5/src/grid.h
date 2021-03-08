#ifndef _GRID_H_
#define _GRID_H_

#include "hit.h"
#include "object3D.h"

typedef bool statetype;

class Grid : public Object3D
{
public:
	Grid(BoundingBox *bb, int nx, int ny, int nz);
	~Grid();
	int getNx();
	int getNy();
	int getNz();
	statetype &getState(int x, int y, int z);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint() const;

private:
	statetype *state;
	int nx, ny, nz;
};

#endif