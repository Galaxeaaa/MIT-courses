#include "camera.h"
#include "group.h"
#include "image.h"
#include "material.h"
#include "matrix.h"
#include "object3d.h"
#include "scene_parser.h"
#include "sphere.h"
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
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input"))
        {
            i++;
            assert(i < argc);
            input_file = argv[i];
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
            output_file = argv[i];
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
            depth_file = argv[i];
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    Image img(width, height);
    Image img_depth(width, height);
    FILE *outfile, *infile, *outfile_depth;
    infile = fopen(input_file, "r");
    outfile = fopen(output_file, "w");
    outfile_depth = fopen(depth_file, "w");

    SceneParser sp(input_file);
    OrthographicCamera *camera = (OrthographicCamera *)sp.getCamera();
    Vec3f background_color = sp.getBackgroundColor();
    Vec3f black(0.0f, 0.0f, 0.0f);
    Group *group = sp.getGroup();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Vec2f pos((float)i / width, (float)j / height);
            Ray ray = camera->generateRay(pos);
            Hit hit(INF, NULL);
            if (group->intersect(ray, hit, camera->getTMin()))
            {
                img.SetPixel(i, j, hit.getMaterial()->getDiffuseColor());
            }
            else
                img.SetPixel(i, j, background_color);
        }
    }
    img.SaveTGA(output_file);

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
    img_depth.SaveTGA(depth_file);

    return 0;
}