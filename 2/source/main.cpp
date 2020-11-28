#include "camera.h"
#include "group.h"
#include "image.h"
#include "material.h"
#include "matrix.h"
#include "object3d.h"
#include "scene_parser.h"
#include "sphere.h"
#include "light.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

Vec3f *map(float depth, float depth_min, float depth_max)
{
    if (depth > depth_max)
        return new Vec3f(0.1f, 0.1f, 0.1f);
    else if (depth < depth_min)
        return new Vec3f(1.0f, 1.0f, 1.0f);
    else
    {
        float f = 1.0f - 0.9f * (depth - depth_min) / (depth_max - depth_min);
        return new Vec3f(f, f, f);
    }
}

int main(int argc, char *argv[])
{
    // input parse
    char *input_filename = NULL;
    int width = 100;
    int height = 100;
    char *output_filename = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_filename = NULL;
    char *normal_filename = NULL;
    bool shade_back = false;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input"))
        {
            i++;
            assert(i < argc);
            input_filename = argv[i];
        }
        else if (!strcmp(argv[i], "-size"))
        {
            i++;
            assert(i < argc);
            width = atoi(argv[i]);
            i++;
            assert(i < argc);
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output"))
        {
            i++;
            assert(i < argc);
            output_filename = argv[i];
        }
        else if (!strcmp(argv[i], "-depth"))
        {
            i++;
            assert(i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_filename = argv[i];
        }
        else if (!strcmp(argv[i], "-normals"))
        {
            i++;
            assert(i < argc);
            normal_filename = argv[i];
        }
        else if (!strcmp(argv[i], "-shade_back"))
        {
            i++;
            assert(i < argc);
            shade_back = true;
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    Image img(width, height);
    Image img_depth(width, height);
    Image img_normal(width, height);
    FILE *outfile, *infile, *outfile_depth, *outfile_normal;
    infile = fopen(input_filename, "r");
    outfile = fopen(output_filename, "w");
    outfile_depth = fopen(depth_filename, "w");
    outfile_normal = fopen(normal_filename, "w");

    SceneParser sp(input_filename);
    OrthographicCamera *camera = (OrthographicCamera *)sp.getCamera();
    Vec3f background_color(sp.getBackgroundColor());
    Vec3f ambient_color(sp.getAmbientLight());
    Vec3f black(0.0f, 0.0f, 0.0f);
    Group *group = sp.getGroup();
    int nlights = sp.getNumLights();

    // OrthographicCamera
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Vec2f pos((float)i / width, (float)j / height);
            Ray ray = camera->generateRay(pos);
            Hit hit(INF, NULL);
            if (group->intersect(ray, hit, camera->getTMin()))
            {
                Vec3f color_object(hit.getMaterial()->getDiffuseColor());
                Vec3f normal(hit.getNormal());
                Vec3f sum, color_pixel;;
                if (hit.getNormal().Dot3(ray.getDirection()) > 0 && shade_back) // back side
                    normal.Negate();
                for (int i = 0; i < nlights; i++)
                {
                    Vec3f color_light, dir_light;
                    float d;
                    Light *light = sp.getLight(i);
                    light->getIllumination(hit.getIntersectionPoint(), dir_light, color_light);
                    if (dir_light.Dot3(normal) < 0)
                        d = - dir_light.Dot3(normal);
                    else
                        d = 0;
                    sum += d * color_light * color_object;
                }
                color_pixel = ambient_color * color_object + sum;
                img.SetPixel(i, j, color_pixel);
            }
            else
                img.SetPixel(i, j, background_color);
        }
    }
    img.SaveTGA(output_filename);

    // Depth visualization
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Vec2f pos((float)i / width, (float)j / height);
            Ray ray = camera->generateRay(pos);
            Hit hit(INF, NULL);
            if (group->intersect(ray, hit, camera->getTMin()))
            {
                Vec3f color(*map(hit.getT(), depth_min, depth_max));
                img_depth.SetPixel(i, j, color);
            }
            else
                img_depth.SetPixel(i, j, black);
        }
    }
    img_depth.SaveTGA(depth_filename);

    // Normal visualization
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Vec2f pos((float)i / width, (float)j / height);
            Ray ray = camera->generateRay(pos);
            Hit hit(INF, NULL);
            if (group->intersect(ray, hit, camera->getTMin()))
            {
                Vec3f color(hit.getNormal());
                color.Set(abs(color.r()), abs(color.g()), abs(color.b()));
                img_depth.SetPixel(i, j, color);
            }
            else
                img_depth.SetPixel(i, j, black);
        }
    }
    img_normal.SaveTGA(normal_filename);

    return 0;
}