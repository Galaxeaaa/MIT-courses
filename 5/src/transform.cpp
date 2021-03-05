#include "transform.h"

int max_depth = 0, cur_depth = 0;

queue<Matrix> q;

Transform::Transform(Matrix &m, Object3D *o) : object(o), mat(m)
{
	/* Set bbox */
	Vec3f min, max;
	o->getBoundingBox()->Get(min, max);
	Vec3f a[8];
	a[0] = min;
	a[1] = Vec3f(min.x(), max.y(), min.z());
	a[2] = Vec3f(max.x(), min.y(), min.z());
	a[3] = Vec3f(max.x(), max.y(), min.z());
	a[4] = Vec3f(min.x(), min.y(), max.z());
	a[5] = Vec3f(min.x(), max.y(), max.z());
	a[6] = Vec3f(max.x(), min.y(), max.z());
	a[7] = max;
	for (int i = 0; i < 8; i++)
	{
		m.Transform(a[i]);
	}
	bbox = new BoundingBox(a[0], a[0]);
	for (int i = 1; i < 8; i++)
	{
		bbox->Extend(a[i]);
	}
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
	cur_depth++;
	max_depth = max_depth < cur_depth ? cur_depth : max_depth;
	Vec3f Ro = r.getOrigin();
	Vec3f Rd = r.getDirection();
	Matrix cp = mat;
	cp.Inverse();
	cp.Transform(Ro);
	cp.TransformDirection(Rd);
	// Rd.Normalize();
	cp.Transpose();
	Ray r_transformed(Ro, Rd);
	float prev_t = h.getT();
	bool isintersect = object->intersect(r_transformed, h, tmin);
	if (isintersect && h.getT() != prev_t) // if t updated and this is not leaf
	{
		if (cur_depth == max_depth)
		{
			while (!q.empty())
				q.pop();
		}
		q.emplace(cp);
	}

	if (cur_depth == 1 && isintersect)
	{
		Vec3f normal = h.getNormal();
		while (!q.empty())
		{
			Matrix mm = q.front();
			q.pop();
			mm.TransformDirection(normal);
			normal.Normalize();
		}
		h.set(h.getT(), h.getMaterial(), normal, r);
		max_depth = 0;
	}

	// cp.TransformDirection(normal);

	cur_depth--;

	return isintersect;
}

void Transform::paint() const
{
	glPushMatrix();
	GLfloat *glMatrix = mat.glGet();
	glMultMatrixf(glMatrix);
	delete[] glMatrix;
	object->paint();
	glPopMatrix();
}

BoundingBox *Transform::getBoundingBox()
{
	BoundingBox *ori_bbx = object->getBoundingBox();
	Vec3f min, max;
	ori_bbx->Get(min, max);
	BoundingBox *bbx = new BoundingBox(Vec3f(), Vec3f());

	return bbx;
}