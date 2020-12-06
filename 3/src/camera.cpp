#include "camera.h"
#include "matrix.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* OrthographicCamera */

OrthographicCamera::OrthographicCamera(const Vec3f &c, const Vec3f &d, const Vec3f &u, const float &size) : center(c), direction(d), up(u), size(size), tmin(0)
{
    direction.Normalize();
    up = up - up.Dot3(direction) * direction;
    up.Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    assert(point.x() >= 0 && point.x() <= 1);
    assert(point.y() >= 0 && point.y() <= 1);

    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    Vec3f origin(center + (point.y() - 0.5) * size * up + (point.x() - 0.5) * size * horizontal);
    Vec3f raydir(direction);
    Ray r(origin, raydir);
    return r;
}

void OrthographicCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-size / 2.0, size / 2.0, -size * (float)h / (float)w / 2.0, size * (float)h / (float)w / 2.0, 0.5, 40.0);
    else
        glOrtho(-size * (float)w / (float)h / 2.0, size * (float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 40.0);
}

void OrthographicCamera::glPlaceCamera(void) { gluLookAt(center.x(), center.y(), center.z(), center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(), up.x(), up.y(), up.z()); }

void OrthographicCamera::dollyCamera(float dist)
{
    center += direction * dist;
    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void OrthographicCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);
    center += horizontal * dx + screenUp * dy;
    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void OrthographicCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;
    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
    rotMat.Transform(center);
    rotMat.TransformDirection(direction);
    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

/* PerspectiveCamera */

PerspectiveCamera::PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle_radians) : center(center), direction(direction), up(up), tmin(0), angle_radians(angle_radians)
{
    up.Normalize();
    direction.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    assert(point.x() >= 0 && point.x() <= 1);
    assert(point.y() >= 0 && point.y() <= 1);

    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    Vec3f origin(center);
    Vec3f raydir(direction + 2 * direction.Length() * tan(angle_radians / 2) * ((point.x() - 0.5) * horizontal + (point.y() - 0.5) * up));
    raydir.Normalize();
    Ray r(origin, raydir);
    return r;
}

void PerspectiveCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle_radians * 180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

void PerspectiveCamera::glPlaceCamera(void) { gluLookAt(center.x(), center.y(), center.z(), center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(), up.x(), up.y(), up.z()); }

void PerspectiveCamera::dollyCamera(float dist)
{
    center += direction * dist;
    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void PerspectiveCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);
    center += horizontal * dx + screenUp * dy;
    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle - ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01)
        ry = tiltAngle - 0.01;
    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
    rotMat.Transform(center);
    rotMat.TransformDirection(direction);
    direction.Normalize();
    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================
}
