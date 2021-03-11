#include "grid.h"
#include "group.h"
#include "triangle.h"

Grid::Grid(BoundingBox *bb, int nx, int ny, int nz) : nx(nx), ny(ny), nz(nz)
{
	bbox = bb;
	state = new statetype[nx * ny * nz];
	memset(state, false, sizeof(statetype));
}

Grid::~Grid()
{
	delete[] state;
}

int Grid::getNx()
{
	return nx;
}

int Grid::getNy()
{
	return ny;
}

int Grid::getNz()
{
	return nz;
}

statetype &Grid::getState(int x, int y, int z) const
{
	return state[x * ny * nz + y * nz + z];
}

void Grid::setState(int x, int y, int z, statetype s)
{
	state[x * ny * nz + y * nz + z] = s;
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
	/* Intersect with BoundingBox first */
	Vec3f bmin(bbox->getMin());
	Vec3f bmax(bbox->getMax());
	Vec3f Ro = r.getOrigin();
	Vec3f Rd = r.getDirection();
	float t_near = -10e8, t_far = 10e8;

	if (Rd.x() == 0 && Ro.x() < bmin.x() && Ro.x() > bmax.x())
		return false; // No Intersection
	if (Rd.y() == 0 && Ro.y() < bmin.y() && Ro.y() > bmax.y())
		return false; // No Intersection
	if (Rd.z() == 0 && Ro.z() < bmin.z() && Ro.z() > bmax.z())
		return false; // No Intersection

	float t1, t2, tmp;
	/* Intersect on x-axis */
	t1 = (bmin.x() - Ro.x()) / Rd.x();
	t2 = (bmax.x() - Ro.x()) / Rd.x();
	if (t1 > t2)
	{
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > t_near)
		t_near = t1;
	if (t2 < t_far)
		t_far = t2;
	/* Intersect on y-axis */
	t1 = (bmin.y() - Ro.y()) / Rd.y();
	t2 = (bmax.y() - Ro.y()) / Rd.y();
	if (t1 > t2)
	{
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > t_near)
		t_near = t1;
	if (t2 < t_far)
		t_far = t2;
	/* Intersect on z-axis */
	t1 = (bmin.z() - Ro.z()) / Rd.z();
	t2 = (bmax.z() - Ro.z()) / Rd.z();
	if (t1 > t2)
	{
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > t_near)
		t_near = t1;
	if (t2 < t_far)
		t_far = t2;

	if (t_near > t_far)
		return false;
	if (t_far < tmin)
		return false;

	/* Intersection with BoundingBox: True */
	MarchingInfo mi;
	initializeRayMarch(mi, r, tmin);
	if (mi.tmin >= 10e8)
	{
		return false;
	}

	Vec3f n;
	switch(mi.face)
	{
		case 0: 
			n.Set(-mi.sign_x, 0, 0);
			break;
		case 1: 
			n.Set(0, -mi.sign_y, 0);
			break;
		case 2: 
			n.Set(0, 0, -mi.sign_z);
			break;
	}

	h.set(mi.tmin, m, n, r);

	return true;
}

void Grid::paint() const
{
	Vec3f min = bbox->getMin();
	Vec3f max = bbox->getMax();
	float dx = (max.x() - min.x()) / nx;
	float dy = (max.y() - min.y()) / ny;
	float dz = (max.z() - min.z()) / nz;
	for (int x = 0; x < nx; x++)
	{
		for (int y = 0; y < ny; y++)
		{
			for (int z = 0; z < nz; z++)
			{
				if (state[x * ny * nz + y * nz + z] == false)
					continue;
				Vec3f grid_vertex[8] = {
					min + Vec3f(x * dx, y * dy, z * dz),
					min + Vec3f(x * dx, (y + 1) * dy, z * dz),
					min + Vec3f((x + 1) * dx, (y + 1) * dy, z * dz),
					min + Vec3f((x + 1) * dx, y * dy, z * dz),
					min + Vec3f(x * dx, y * dy, (z + 1) * dz),
					min + Vec3f(x * dx, (y + 1) * dy, (z + 1) * dz),
					min + Vec3f((x + 1) * dx, (y + 1) * dy, (z + 1) * dz),
					min + Vec3f((x + 1) * dx, y * dy, (z + 1) * dz)};
				Triangle(grid_vertex[0], grid_vertex[1], grid_vertex[2], m).paint();
				Triangle(grid_vertex[0], grid_vertex[2], grid_vertex[3], m).paint();
				Triangle(grid_vertex[0], grid_vertex[5], grid_vertex[1], m).paint();
				Triangle(grid_vertex[0], grid_vertex[4], grid_vertex[5], m).paint();
				Triangle(grid_vertex[1], grid_vertex[5], grid_vertex[2], m).paint();
				Triangle(grid_vertex[2], grid_vertex[5], grid_vertex[6], m).paint();
				Triangle(grid_vertex[2], grid_vertex[6], grid_vertex[3], m).paint();
				Triangle(grid_vertex[3], grid_vertex[6], grid_vertex[7], m).paint();
				Triangle(grid_vertex[0], grid_vertex[3], grid_vertex[4], m).paint();
				Triangle(grid_vertex[4], grid_vertex[3], grid_vertex[7], m).paint();
				Triangle(grid_vertex[4], grid_vertex[6], grid_vertex[5], m).paint();
				Triangle(grid_vertex[4], grid_vertex[7], grid_vertex[6], m).paint();
			}
		}
	}
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const
{
	Vec3f Ro = r.getOrigin();
	Vec3f Rd = r.getDirection();
	// Ray Origin in Box
	mi.tmin = 10e8;
	mi.d_tx = abs(((bbox->getMax().x() - bbox->getMin().x()) / nx) * (Rd.Length() / Rd.x()));
	mi.d_ty = abs(((bbox->getMax().y() - bbox->getMin().y()) / ny) * (Rd.Length() / Rd.y()));
	mi.d_tz = abs(((bbox->getMax().z() - bbox->getMin().z()) / nz) * (Rd.Length() / Rd.z()));
	mi.sign_x = Rd.x() > 0 ? 1 : (Rd.x() < 0 ? -1 : 0);
	mi.sign_y = Rd.y() > 0 ? 1 : (Rd.y() < 0 ? -1 : 0);
	mi.sign_z = Rd.z() > 0 ? 1 : (Rd.z() < 0 ? -1 : 0);

	/* Apply Ray-Box Intersection to decide the first cell. */
	Vec3f bmin = bbox->getMin();
	Vec3f bmax = bbox->getMax();
	float dx = (bmax.x() - bmin.x()) / nx;
	float dy = (bmax.y() - bmin.y()) / ny;
	float dz = (bmax.z() - bmin.z()) / nz;
	int x, y, z;
	for (x = 0; x < nx; x++)
	{
		for (y = 0; y < ny; y++)
		{
			for (z = 0; z < nz; z++)
			{
				if (getState(x, y, z) == false)
					continue;
				/* Cell range */
				Vec3f cmin(bmin + Vec3f(x * dx, y * dy, z * dz));
				Vec3f cmax(bmin + Vec3f((x + 1) * dx, (y + 1) * dy, (z + 1) * dz));
				float t_near = -10e8, t_far = 10e8;
				float cddt_next_x, cddt_next_y, cddt_next_z;
				int far_face, near_face;

				/* Ray-Box Intersection */
				if (Rd.x() == 0 && Ro.x() < cmin.x() && Ro.x() > cmax.x())
					continue; // No Intersection
				if (Rd.y() == 0 && Ro.y() < cmin.y() && Ro.y() > cmax.y())
					continue; // No Intersection
				if (Rd.z() == 0 && Ro.z() < cmin.z() && Ro.z() > cmax.z())
					continue; // No Intersection

				float t1, t2, tmp;
				/* Intersect on x-axis */
				t1 = (cmin.x() - Ro.x()) / Rd.x();
				t2 = (cmax.x() - Ro.x()) / Rd.x();
				if (t1 > t2)
				{
					tmp = t1;
					t1 = t2;
					t2 = tmp;
				}
				cddt_next_x = t2;
				if (t1 > t_near)
				{
					t_near = t1;
					near_face = 0;
				}
				if (t2 < t_far)
				{
					t_far = t2;
					far_face = 0;
				}
				/* Intersect on y-axis */
				t1 = (cmin.y() - Ro.y()) / Rd.y();
				t2 = (cmax.y() - Ro.y()) / Rd.y();
				if (t1 > t2)
				{
					tmp = t1;
					t1 = t2;
					t2 = tmp;
				}
				cddt_next_y = t2;
				if (t1 > t_near)
				{
					t_near = t1;
					near_face = 1;
				}
				if (t2 < t_far)
				{
					t_far = t2;
					far_face = 1;
				}
				/* Intersect on z-axis */
				t1 = (cmin.z() - Ro.z()) / Rd.z();
				t2 = (cmax.z() - Ro.z()) / Rd.z();
				if (t1 > t2)
				{
					tmp = t1;
					t1 = t2;
					t2 = tmp;
				}
				cddt_next_z = t2;
				if (t1 > t_near)
				{
					t_near = t1;
					near_face = 2;
				}
				if (t2 < t_far)
				{
					t_far = t2;
					far_face = 2;
				}

				if (t_near > t_far)
					continue; // Box Missed
				if (t_far < tmin)
					continue; // Box Behind

				if (t_near > tmin)
				{
					if (mi.tmin > t_near)
					{
						mi.tmin = t_near;
						mi.i = x;
						mi.j = y;
						mi.k = z;
						mi.t_next_x = cddt_next_x;
						mi.t_next_y = cddt_next_y;
						mi.t_next_z = cddt_next_z;
						mi.face = near_face;
					}
				}
				else
				{
					if (mi.tmin > t_far)
					{
						mi.tmin = t_far;
						mi.i = x;
						mi.j = y;
						mi.k = z;
						mi.t_next_x = cddt_next_x;
						mi.t_next_y = cddt_next_y;
						mi.t_next_z = cddt_next_z;
						mi.face = far_face;
					}
				}
			}
		}
	}
}