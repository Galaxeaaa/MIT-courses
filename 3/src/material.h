#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "hit.h"
#include "ray.h"
#include "vectors.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material
{

public:
	// CONSTRUCTORS & DESTRUCTOR
	Material(const Vec3f &d_color) : diffuseColor(d_color) {}
	virtual ~Material() {}
	virtual Material *clone() const = 0;

	// ACCESSORS
	virtual Vec3f getDiffuseColor() const { return diffuseColor; }
	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;
	virtual void glSetMaterial(void) const = 0;

protected:
	// REPRESENTATION
	Vec3f diffuseColor;
};

class PhongMaterial : public Material
{
public:
	PhongMaterial::PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent);
	virtual Material *clone() const;

	virtual Vec3f getSpecularColor() const { return specularColor; }
	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
	virtual void glSetMaterial(void) const;

protected:
	Vec3f specularColor;
	float exponent;
};

// ====================================================================
// ====================================================================

#endif