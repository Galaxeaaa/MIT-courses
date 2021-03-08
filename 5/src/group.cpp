#include "group.h"

Group::Group(int n) : n(n)
{
	instances = new Object3D *[n];
}

Group::~Group()
{
	delete[] instances;
}

int Group::getN()
{
	return n;
}

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
	bool is = false;
	for (int i = 0; i < n; i++)
	{
		if (instances[i]->intersect(r, h, tmin))
			is = true;
	}
	return is;
}

void Group::addObject(int index, Object3D *obj)
{
	if (index >= n)
	{
		Object3D **new_instances = new Object3D *[n];
		memcpy(new_instances, instances, n * sizeof(Object3D *));
		delete[] instances;
		instances = new Object3D *[n + 1];
		memcpy(instances, new_instances, n * sizeof(Object3D *));
		delete [] new_instances;
		n++;
	}
	instances[index] = obj;
	BoundingBox *obox = obj->getBoundingBox();
	if (obox)
		if (!bbox)
			bbox = obox;
		else
			bbox->Extend(obox);
}

void Group::paint() const
{
	for (int i = 0; i < n; i++)
	{
		instances[i]->paint();
	}
}

void Group::insertIntoGrid(Grid *g, Matrix *m)
{
	for (int i = 0; i < n; i++)
	{
		instances[i]->insertIntoGrid(g, m);
	}
}