#pragma once

#include "hit.h"
#include "ray.h"
#include "scene_parser.h"
#include "vectors.h"

class RayTracer
{
public:
    /* CONSTRUCTOR */
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows);

    /* MAIN METHOD */
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;

    static Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming);
    static void transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted);

private:
    SceneParser *s;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
};