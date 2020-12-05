#ifndef _CAMERA_H_
#define _CAMERA_H_
#define INF 10e8

#include "ray.h"
#include "vectors.h"

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
    OrthographicCamera(const Vec3f &c, const Vec3f &d, const Vec3f &u, const float &size) : center(c), direction(d), up(u), size(size), tmin(0)
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
    float getTMin() const { return tmin; }

protected:
    Vec3f center, direction, up, horizontal;
    float size, tmin;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle_radians) : center(center), direction(direction), up(up), tmin(0), angle_radians(angle_radians)
    {
        up.Normalize();
        direction.Normalize();
        Vec3f::Cross3(horizon, direction, up);
        horizon.Normalize();
    }

    Ray generateRay(Vec2f point)
    {
        assert(point.x() >= 0 && point.x() <= 1);
        assert(point.y() >= 0 && point.y() <= 1);

        Vec3f origin(center);
        Vec3f raydir(direction + 2 * direction.Length() * tan(angle_radians / 2) * ((point.x() - 0.5) * horizon + (point.y() - 0.5) * up));
        raydir.Normalize();
        Ray r(origin, raydir);
        return r;
    }
    float getTMin() const { return tmin; };

protected:
    Vec3f center, direction, up, horizon;
    float tmin, angle_radians;
};

#endif