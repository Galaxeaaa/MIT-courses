#pragma once_
#define INF 10e8

#include "ray.h"
#include "vectors.h"

class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

    // ====================================================================
    // dollyCamera, truckCamera, and RotateCamera
    //
    // Asumptions:
    // - up is really up (i.e., it hasn't been changed
    // to point to "screen up")
    // - up and direction are normalized
    // Special considerations:
    // - If your constructor precomputes any vectors for
    // use in 'generateRay', you will likely to recompute those
    // values at athe end of the these three routines
    // ====================================================================
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const Vec3f &c, const Vec3f &d, const Vec3f &u, const float &size);
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const { return tmin; }

    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

protected:
    Vec3f center, direction, up;
    float size, tmin;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle_radians);
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const { return tmin; };

    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

protected:
    Vec3f center, direction, up;
    float tmin, angle_radians;
};
