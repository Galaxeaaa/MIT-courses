#include "group.h"

Group::Group(int n) : n(n)
{
	instances = new Object3D *[n];
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