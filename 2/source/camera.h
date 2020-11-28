#ifndef _CAMERA_H_
#define _CAMERA_H_
#define INF 10e8

#include "vectors.h"
#include "ray.h"

class Camera
{
public:
    Camera(){};
    ~Camera(){};
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const Vec3f &c, const Vec3f &d, const Vec3f &u, const float &size) : center(c), size(size), direction(d), up(u), tmin(-INF)
    {
        direction.Normalize();

        up = up - up.Dot3(direction) * direction;
        up.Normalize();

        Vec3f::Cross3(horizontal, direction, up);
    }
    Ray generateRay(Vec2f point)
    {
        assert(point.x() >= 0 && point.x() <= 1);
        assert(point.y() >= 0 && point.y() <= 1);

        Vec3f origin(center + (point.y() - 0.5) * size * up + (point.x() - 0.5) * size * horizontal);
        Vec3f raydir(direction);
        Ray r(origin, raydir);
        return r;
    }
    float getTMin() const
    {
        return tmin;
    }

protected:
    Vec3f center, direction, up, horizontal;
    float size, tmin;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(){};
    PerspectiveCamera(Vec3f , Vec3f, Vec3f, Vec3f){};
    ~PerspectiveCamera(){};
    Ray generateRay(Vec2f point);
    float getTMin() const

protected:
    Vec3f center, direction, up, horizontal;
    float size, tmin;
};

#endif