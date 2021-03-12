#ifndef _GRID_H_
#define _GRID_H_

#include "group.h"
#include "hit.h"
#include "object3D.h"
#include "MarchingInfo.h"

typedef bool statetype;

class Grid : public Object3D
{
public:
	Grid(BoundingBox *bb, int nx, int ny, int nz);
	~Grid();
	int getNx();
	int getNy();
	int getNz();
	bool isInBox(int x, int y, int z) const;
	statetype &getState(int x, int y, int z) const;
	void setState(int x, int y, int z, statetype s);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint() const;
	void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

private:
	statetype *state;
	int nx, ny, nz;
	Group *g;

	static void rayBoxIntersection(Vec3f min, Vec3f max, const Ray &r, int tmin);
};

#endif