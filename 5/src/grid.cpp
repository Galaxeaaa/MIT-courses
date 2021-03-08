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

statetype &Grid::getState(int x, int y, int z)
{
	return state[x * ny * nz + y * nz + z];
}

void Grid::setGroup()
{
	int o = 0;
	int cnt = 0;
	for (int i = 0; i < nx * ny * nz; i++)
	{
		if (state[i] == true)
			o++;
	}
	g = new Group(o);
	Material *new_m = new PhongMaterial(Vec3f(1.0, 1.0, 1.0), Vec3f(1.0, 1.0, 1.0), 0.1, Vec3f(0.0, 0.5, 0.0), Vec3f(), 1);
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
				Group *grid1 = new Group(12);
				Vec3f grid_vertex[8] = {
					min + Vec3f(x * dx, y * dy, z * dz),
					min + Vec3f(x * dx, (y + 1) * dy, z * dz),
					min + Vec3f((x + 1) * dx, (y + 1) * dy, z * dz),
					min + Vec3f((x + 1) * dx, y * dy, z * dz),
					min + Vec3f(x * dx, y * dy, (z + 1) * dz),
					min + Vec3f(x * dx, (y + 1) * dy, (z + 1) * dz),
					min + Vec3f((x + 1) * dx, (y + 1) * dy, (z + 1) * dz),
					min + Vec3f((x + 1) * dx, y * dy, (z + 1) * dz)
				};
				Triangle *nTri[12];
				nTri[0] = new Triangle(grid_vertex[0], grid_vertex[1], grid_vertex[2], new_m);
				grid1->addObject(0, nTri[0]);
				nTri[1] = new Triangle(grid_vertex[0], grid_vertex[2], grid_vertex[3], new_m);
				grid1->addObject(1, nTri[1]);
				nTri[2] = new Triangle(grid_vertex[0], grid_vertex[5], grid_vertex[1], new_m);
				grid1->addObject(2, nTri[2]);
				nTri[3] = new Triangle(grid_vertex[0], grid_vertex[4], grid_vertex[5], new_m);
				grid1->addObject(3, nTri[3]);
				nTri[4] = new Triangle(grid_vertex[1], grid_vertex[5], grid_vertex[2], new_m);
				grid1->addObject(4, nTri[4]);
				nTri[5] = new Triangle(grid_vertex[2], grid_vertex[5], grid_vertex[6], new_m);
				grid1->addObject(5, nTri[5]);
				nTri[6] = new Triangle(grid_vertex[2], grid_vertex[6], grid_vertex[3], new_m);
				grid1->addObject(6, nTri[6]);
				nTri[7] = new Triangle(grid_vertex[3], grid_vertex[6], grid_vertex[7], new_m);
				grid1->addObject(7, nTri[7]);
				nTri[8] = new Triangle(grid_vertex[0], grid_vertex[3], grid_vertex[4], new_m);
				grid1->addObject(8, nTri[8]);
				nTri[9] = new Triangle(grid_vertex[4], grid_vertex[3], grid_vertex[7], new_m);
				grid1->addObject(9, nTri[9]);
				nTri[10] = new Triangle(grid_vertex[4], grid_vertex[6], grid_vertex[5], new_m);
				grid1->addObject(10, nTri[10]);
				nTri[11] = new Triangle(grid_vertex[4], grid_vertex[7], grid_vertex[6], new_m);
				grid1->addObject(11, nTri[11]);

				g->addObject(cnt++, grid1);
			}
		}
	}
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
	return g->intersect(r, h, tmin);
}

void Grid::paint() const
{
	Material *new_m = new PhongMaterial(Vec3f(1.0, 1.0, 1.0), Vec3f(1.0, 1.0, 1.0), 0.1, Vec3f(0.0, 0.5, 0.0), Vec3f(), 1);
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
				Triangle(grid_vertex[0], grid_vertex[1], grid_vertex[2], new_m).paint();
				Triangle(grid_vertex[0], grid_vertex[2], grid_vertex[3], new_m).paint();
				Triangle(grid_vertex[0], grid_vertex[5], grid_vertex[1], new_m).paint();
				Triangle(grid_vertex[0], grid_vertex[4], grid_vertex[5], new_m).paint();
				Triangle(grid_vertex[1], grid_vertex[5], grid_vertex[2], new_m).paint();
				Triangle(grid_vertex[2], grid_vertex[5], grid_vertex[6], new_m).paint();
				Triangle(grid_vertex[2], grid_vertex[6], grid_vertex[3], new_m).paint();
				Triangle(grid_vertex[3], grid_vertex[6], grid_vertex[7], new_m).paint();
				Triangle(grid_vertex[0], grid_vertex[3], grid_vertex[4], new_m).paint();
				Triangle(grid_vertex[4], grid_vertex[3], grid_vertex[7], new_m).paint();
				Triangle(grid_vertex[4], grid_vertex[6], grid_vertex[5], new_m).paint();
				Triangle(grid_vertex[4], grid_vertex[7], grid_vertex[6], new_m).paint();
			}
		}
	}
}