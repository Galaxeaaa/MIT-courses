#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "hit.h"
#include "ray.h"
#include "vectors.h"

// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight
// artifact for small specular exponents (wide specular lobe)
// ====================================================================

// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "glCanvas.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

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
	PhongMaterial(const Vec3f &diffuseColor,
				  const Vec3f &specularColor,
				  float exponent,
				  const Vec3f &reflectiveColor,
				  const Vec3f &transparentColor,
				  float indexOfRefraction);
	virtual Material *clone() const;

	Vec3f getSpecularColor() const { return specularColor; }
	float getExponent() const { return exponent; }
	Vec3f getReflectiveColor() const { return reflectiveColor; }
	Vec3f getTransparentColor() const { return transparentColor; }
	float getIndexOfRefraction() const { return indexOfRefraction; }
	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
	virtual void glSetMaterial(void) const;

protected:
	Vec3f specularColor;
	float exponent;
	Vec3f reflectiveColor;
	Vec3f transparentColor;
	float indexOfRefraction;
};

// ====================================================================
// ====================================================================

#endif