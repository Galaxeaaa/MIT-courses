#include "grid.h"
#include "triangle.h"

Grid::Grid(BoundingBox *bb, int nx, int ny, int nz): nx(nx), ny(ny), nz(nz)
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

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
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
		for (int y = 0; y < ny; x++)
		{
			for (int z = 0; z < nz; x++)
			{
				Vec3f grid_vertex[8] = {
					Vec3f(x * dx, y * dy, z * dz),
					Vec3f(x * dx, (y + 1) * dy, z * dz),
					Vec3f((x + 1) * dx, (y + 1) * dy, z * dz),
					Vec3f((x + 1) * dx, y * dy, z * dz),
					Vec3f(x * dx, y * dy, (z + 1) * dz),
					Vec3f(x * dx, (y + 1) * dy, (z + 1) * dz),
					Vec3f((x + 1) * dx, (y + 1) * dy, (z + 1) * dz),
					Vec3f((x + 1) * dx, y * dy, (z + 1) * dz)
				};
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