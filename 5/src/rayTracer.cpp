#include "rayTracer.h"
#include "camera.h" // use its INF
#include "group.h"
#include "hit.h"
#include "light.h"
#include "material.h"
#include "ray.h"
#include "rayTree.h"
#include "scene_parser.h"

#define EPSILON 0.001

RayTracer::RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows)
    : s(s), max_bounces(max_bounces), cutoff_weight(cutoff_weight), shadows(shadows)
{
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const
{
    if (bounces == -1)
    {
        return Vec3f();
    }
    else if (weight < cutoff_weight)
    {
        return Vec3f();
    }
    else if (s->getGroup()->intersect(ray, hit, EPSILON))
    {
        Ray n(hit.getIntersectionPoint(), hit.getNormal());
        RayTree::AddShadowSegment(n, 0, 1);
        Vec3f color = s->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
        /* Shadowing */
		for (int i = 0; i < s->getNumLights(); i++)
		{
			Vec3f dir_to_light, light_color;
			float dist_to_light;
			s->getLight(i)->getIllumination(hit.getIntersectionPoint(), dir_to_light, light_color, dist_to_light);
			Ray ray2(hit.getIntersectionPoint(), dir_to_light);
			Hit hit2(dist_to_light, NULL, Vec3f(), ray2);
			if (shadows)
			{
				s->getGroup()->intersect(ray2, hit2, EPSILON); // CAN BE ACCELERATED
				if (hit2.getT() == dist_to_light)
				{
					color += hit.getMaterial()->Shade(ray, hit, dir_to_light, light_color);
				}
			}
			else
			{
				color += hit.getMaterial()->Shade(ray, hit, dir_to_light, light_color);
			}
			// RayTree::AddShadowSegment(ray2, 0, hit2.getT());
		}
        if (bounces > 0)
        {
            /* Reflecting */
            if (((PhongMaterial *)hit.getMaterial())->getReflectiveColor() != Vec3f(0, 0, 0))
            {
                Ray ray3(hit.getIntersectionPoint(), mirrorDirection(hit.getNormal(), ray.getDirection())); // CAN BE MORE REALISTIC
                Hit hit3(INF, NULL, Vec3f(), ray3);
                color += traceRay(ray3, tmin, bounces - 1, weight * ((PhongMaterial *)hit.getMaterial())->getReflectiveColor().Length(), indexOfRefraction, hit3) * ((PhongMaterial *)hit.getMaterial())->getReflectiveColor();
                RayTree::AddReflectedSegment(ray3, 0, hit3.getT());
            }
            /* Transmitting */
            if (((PhongMaterial *)hit.getMaterial())->getTransparentColor() != Vec3f(0, 0, 0))
            {
                float index = ((PhongMaterial *)hit.getMaterial())->getIndexOfRefraction();
                float next_index;
                Vec3f ray4_dir;
                if (ray.getDirection().Dot3(hit.getNormal()) < 0)
                {
                    transmittedDirection(hit.getNormal(), ray.getDirection(), indexOfRefraction, index, ray4_dir);
                    next_index = index;
                }
                else
                {
                    transmittedDirection(hit.getNormal(), ray.getDirection(), indexOfRefraction, 1, ray4_dir);
                    next_index = 1;
                }
                if (ray4_dir != Vec3f())
                {
                    Ray ray4(hit.getIntersectionPoint(), ray4_dir);
                    Hit hit4(INF, NULL, Vec3f(), ray4);
                    color += traceRay(ray4, tmin, bounces - 1, weight * ((PhongMaterial *)hit.getMaterial())->getTransparentColor().Length(), next_index, hit4) * ((PhongMaterial *)hit.getMaterial())->getTransparentColor();
                    RayTree::AddTransmittedSegment(ray4, 0, hit4.getT());
                }
            }
        }
        // else
        // {
        // 	color += ((PhongMaterial *)hit.getMaterial())->Shade(ray, hit, );
        // }

        if (bounces == max_bounces)
            RayTree::SetMainSegment(ray, 0, hit.getT());

        return color;
    }
    else
    {
        return s->getBackgroundColor();
    }
}

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming)
{
    return Vec3f(incoming - 2 * incoming.Dot3(normal) * normal);
}

void RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted)
{
    Vec3f N(normal), I(incoming);
    N.Normalize();
    I.Normalize();

    if (1 - (index_i / index_t) * (index_i / index_t) * (1 - (N.Dot3(-1 * I)) * (N.Dot3(-1 * I))) < 0)
    {
        transmitted = Vec3f();
    }
    else
    {
        if (I.Dot3(N) > 0)
            N.Negate();
        transmitted = (index_i / index_t * (N.Dot3(-1 * I)) - sqrt(1 - (index_i / index_t) * (index_i / index_t) * (1 - (N.Dot3(-1 * I)) * (N.Dot3(-1 * I))))) * N + index_i / index_t * I;
    }
}
