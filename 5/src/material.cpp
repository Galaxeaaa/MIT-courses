#include "material.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <algorithm>
#include <math.h>

PhongMaterial::PhongMaterial(const Vec3f &diffuseColor,
                                            const Vec3f &specularColor,
                                            float exponent,
                                            const Vec3f &reflectiveColor,
                                            const Vec3f &transparentColor,
                                            float indexOfRefraction)
    : Material(diffuseColor),
      specularColor(specularColor),
      exponent(exponent),
      reflectiveColor(reflectiveColor),
      transparentColor(transparentColor),
      indexOfRefraction(indexOfRefraction)
{
}

extern bool shade_back;

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    PhongMaterial *m = (PhongMaterial *)hit.getMaterial();
    Vec3f diffuse_color(m->getDiffuseColor());
    Vec3f specular_color(m->getSpecularColor());
    Vec3f l = dirToLight;
    Vec3f v = -1.0 * ray.getDirection();
    Vec3f n(hit.getNormal());
    Vec3f h(l + v);
    h.Normalize();

    if (n.Dot3(v) < 0 && shade_back)
    {
        n.Negate();
    }

    Vec3f Lo = (diffuse_color * max(n.Dot3(l), 0.0f) + specular_color * pow(n.Dot3(h), exponent) * max(n.Dot3(l), 0.0f)) * lightColor;

    return Lo;
}

Material *PhongMaterial::clone() const
{
	PhongMaterial *test;
    return (test = new PhongMaterial(*this));
}

void PhongMaterial::glSetMaterial(void) const
{
    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {
        getSpecularColor().r(),
        getSpecularColor().g(),
        getSpecularColor().b(),
        1.0};
    GLfloat diffuse[4] = {
        getDiffuseColor().r(),
        getDiffuseColor().g(),
        getDiffuseColor().b(),
        1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glexponent = exponent;
    if (glexponent < 0)
        glexponent = 0;
    if (glexponent > 128)
        glexponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight
    // artifact for small specular exponents (wide specular lobe)

    if (SPECULAR_FIX_WHICH_PASS == 0)
    {
        // First pass, draw only the specular highlights
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
    }
    else if (SPECULAR_FIX_WHICH_PASS == 1)
    {
        // Second pass, compute normal dot light
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }
    else
    {
        // Third pass, add ambient & diffuse terms
        assert(SPECULAR_FIX_WHICH_PASS == 2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }

#endif
}