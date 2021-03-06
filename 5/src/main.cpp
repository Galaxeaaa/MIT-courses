#include "camera.h"
#include "glCanvas.h"
#include "group.h"
#include "image.h"
#include "light.h"
#include "material.h"
#include "matrix.h"
#include "object3d.h"
#include "rayTracer.h"
#include "rayTree.h"
#include "scene_parser.h"
#include "sphere.h"
#include <fstream>
#include <io.h>
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

int theta_steps = 20, phi_steps = 20;
char input_filename[MAX_PARSER_TOKEN_LENGTH] = "./input_file/";
int width = 100;
int height = 100;
char output_filename[MAX_PARSER_TOKEN_LENGTH] = "./output_file/";
float depth_min = 0;
float depth_max = 1;
char depth_filename[MAX_PARSER_TOKEN_LENGTH] = "./output_file/";
char normal_filename[MAX_PARSER_TOKEN_LENGTH] = "./output_file/";
bool shade_back = false;
bool draw_depth = false;
bool draw_normal = false;
bool draw_tessellation = false;
bool gui = false;
bool gouraud = false;
bool shadows = false;
int bounces = 0;
float cutoff_weight = 0;
SceneParser *sp;
/* Grid */
int nx = 0, ny = 0, nz = 0;
bool grid = false;
bool visualize_grid = false;
Grid *g = nullptr;

void renderFunc()
{
	Image img(width, height);
	Image img_depth(width, height);
	Image img_normal(width, height);
	FILE *outfile, *outfile_depth, *outfile_normal;

	Camera *camera = (Camera *)sp->getCamera();
	Vec3f background_color(sp->getBackgroundColor());
	Vec3f ambient_color(sp->getAmbientLight());
	Vec3f black(0.0f, 0.0f, 0.0f);
	Group *group = sp->getGroup();
	int nlights = sp->getNumLights();

	if (visualize_grid)
	{
		sp->getGroup()->addObject(sp->getGroup()->getN(), g);
	}

	// Camera
	// OrthographicCamera
	outfile = fopen(output_filename, "w");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vec2f pos((float)i / width, (float)j / height);
			Ray ray = camera->generateRay(pos);
			Hit hit(INF, NULL, Vec3f(0, 0, 0), ray);
			RayTracer rt(sp, bounces, cutoff_weight, shadows);
			Vec3f color = rt.traceRay(ray, INF, bounces, 1, 1, hit);
			img.SetPixel(i, j, color);
		}
	}
	img.SaveTGA(output_filename);

	// Depth visualization
	if (draw_depth)
	{
		outfile_depth = fopen(depth_filename, "w");
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec2f pos((float)i / width, (float)j / height);
				Ray ray = camera->generateRay(pos);
				Hit hit(INF, NULL, Vec3f(0, 0, 0), ray);
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
	}

	// Normal visualization
	if (draw_normal)
	{
		outfile_normal = fopen(normal_filename, "w");
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec2f pos((float)i / width, (float)j / height);
				Ray ray = camera->generateRay(pos);
				Hit hit(INF, NULL, Vec3f(0, 0, 0), ray);
				if (group->intersect(ray, hit, camera->getTMin()))
				{
					Vec3f color(hit.getNormal());
					color.Set(abs(color.r()), abs(color.g()), abs(color.b()));
					img_normal.SetPixel(i, j, color);
				}
				else
					img_normal.SetPixel(i, j, black);
			}
		}
		img_normal.SaveTGA(normal_filename);
	}
}

void traceRayFunc(float x, float y)
{
	Camera *camera = (Camera *)sp->getCamera();
	Vec3f background_color(sp->getBackgroundColor());
	Vec3f ambient_color(sp->getAmbientLight());
	Vec3f black(0.0f, 0.0f, 0.0f);
	Group *group = sp->getGroup();
	int nlights = sp->getNumLights();

	Vec2f pos((float)x, (float)y);
	Ray ray = camera->generateRay(pos);
	Hit hit(INF, NULL, Vec3f(), ray);
	if (!visualize_grid)
	{
		RayTracer rt(sp, bounces, cutoff_weight, shadows);
		rt.traceRay(ray, INF, bounces, 1, 1, hit);
	}
	else
	{
		Material *pm(sp->getMaterial(0));
        RayTree::SetMainSegment(ray, 0, INF);

		MarchingInfo mi;
		g->initializeRayMarch(mi, ray, 0);
		if (mi.tmin >= 10e8)
		{
			RayTree::paint();
			return;
		}

		Vec3f min = g->getBoundingBox()->getMin();
		Vec3f max = g->getBoundingBox()->getMax();
		float dx = (max.x() - min.x()) / nx;
		float dy = (max.y() - min.y()) / ny;
		float dz = (max.z() - min.z()) / nz;

		while (1)
		{
			if (!g->isInBox(mi.i, mi.j, mi.k))
			{
				RayTree::paint();
				return;
			}
			if (g->getState(mi.i, mi.j, mi.k) == true)
			{
				Vec3f grid_vertex[8] = {
					g->getBoundingBox()->getMin() + Vec3f(mi.i * dx, mi.j * dy, mi.k * dz),
					g->getBoundingBox()->getMin() + Vec3f(mi.i * dx, (mi.j + 1) * dy, mi.k * dz),
					g->getBoundingBox()->getMin() + Vec3f((mi.i + 1) * dx, (mi.j + 1) * dy, mi.k * dz),
					g->getBoundingBox()->getMin() + Vec3f((mi.i + 1) * dx, mi.j * dy, mi.k * dz),
					g->getBoundingBox()->getMin() + Vec3f(mi.i * dx, mi.j * dy, (mi.k + 1) * dz),
					g->getBoundingBox()->getMin() + Vec3f(mi.i * dx, (mi.j + 1) * dy, (mi.k + 1) * dz),
					g->getBoundingBox()->getMin() + Vec3f((mi.i + 1) * dx, (mi.j + 1) * dy, (mi.k + 1) * dz),
					g->getBoundingBox()->getMin() + Vec3f((mi.i + 1) * dx, mi.j * dy, (mi.k + 1) * dz)};

				RayTree::AddHitCellFace(grid_vertex[0], grid_vertex[1], grid_vertex[2], grid_vertex[3], Vec3f(0, 0, -1), pm);
				RayTree::AddHitCellFace(grid_vertex[0], grid_vertex[4], grid_vertex[5], grid_vertex[1], Vec3f(-1, 0, 0), pm);
				RayTree::AddHitCellFace(grid_vertex[0], grid_vertex[3], grid_vertex[7], grid_vertex[4], Vec3f(0, -1, 0), pm);
				RayTree::AddHitCellFace(grid_vertex[7], grid_vertex[6], grid_vertex[5], grid_vertex[4], Vec3f(0, 0, 1), pm);
				RayTree::AddHitCellFace(grid_vertex[7], grid_vertex[3], grid_vertex[2], grid_vertex[6], Vec3f(1, 0, 0), pm);
				RayTree::AddHitCellFace(grid_vertex[1], grid_vertex[5], grid_vertex[6], grid_vertex[2], Vec3f(0, 1, 0), pm);

				switch (mi.face)
				{
				case 0:
					if (mi.sign_x == 1)
						RayTree::AddEnteredFace(grid_vertex[0], grid_vertex[4], grid_vertex[5], grid_vertex[1], Vec3f(-1, 0, 0), pm);
					else
						RayTree::AddEnteredFace(grid_vertex[7], grid_vertex[3], grid_vertex[2], grid_vertex[6], Vec3f(1, 0, 0), pm);
					break;
				case 1:
					if (mi.sign_y == 1)
						RayTree::AddEnteredFace(grid_vertex[0], grid_vertex[3], grid_vertex[7], grid_vertex[4], Vec3f(0, -1, 0), pm);
					else
						RayTree::AddEnteredFace(grid_vertex[1], grid_vertex[5], grid_vertex[6], grid_vertex[2], Vec3f(0, 1, 0), pm);
					break;
				case 2:
					if (mi.sign_z == 1)
						RayTree::AddEnteredFace(grid_vertex[0], grid_vertex[1], grid_vertex[2], grid_vertex[3], Vec3f(0, 0, -1), pm);
					else
						RayTree::AddEnteredFace(grid_vertex[7], grid_vertex[6], grid_vertex[5], grid_vertex[4], Vec3f(0, 0, 1), pm);
					break;
				}
			}
			mi.nextCell();
		}
	}
	RayTree::paint();
}

int main(int argc, char *argv[])
{
	/* input parse */
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-input"))
		{
			i++;
			assert(i < argc);
			strcat(input_filename, argv[i]);
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
			strcat(output_filename, argv[i]);
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
			strcat(depth_filename, argv[i]);
			draw_depth = true;
		}
		else if (!strcmp(argv[i], "-normals"))
		{
			i++;
			assert(i < argc);
			strcat(normal_filename, argv[i]);
			draw_normal = true;
		}
		else if (!strcmp(argv[i], "-shade_back"))
		{
			shade_back = true;
		}
		else if (!strcmp(argv[i], "-tessellation"))
		{
			i++;
			assert(i < argc);
			theta_steps = atoi(argv[i]);
			i++;
			assert(i < argc);
			phi_steps = atoi(argv[i]);
			draw_tessellation = true;
		}
		else if (!strcmp(argv[i], "-gui"))
		{
			gui = true;
		}
		else if (!strcmp(argv[i], "-gouraud"))
		{
			gouraud = true;
		}
		else if (!strcmp(argv[i], "-shadows"))
		{
			shadows = true;
		}
		else if (!strcmp(argv[i], "-bounces"))
		{
			i++;
			bounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight"))
		{
			i++;
			cutoff_weight = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-grid"))
		{
			i++;
			nx = atoi(argv[i]);
			i++;
			ny = atoi(argv[i]);
			i++;
			nz = atoi(argv[i]);
			grid = true;
		}
		else if (!strcmp(argv[i], "-visualize_grid"))
		{
			visualize_grid = true;
		}
		else
		{
			std::cout << "whoops error with command line argument" << i << ": \'" << argv[i] << std::endl;
			assert(0);
		}
	}

	FILE *infile;
	infile = fopen(input_filename, "r");
	// Check if output_file/ exists. If not, make directory.
	if (access("output_file/", 0) != 0)
		system("md output_file");

	sp = new SceneParser(input_filename);

	if (grid)
	{
		BoundingBox *gb = sp->getGroup()->getBoundingBox();
		g = new Grid(gb, nx, ny, nz);
		Matrix *m = new Matrix();
		sp->getGroup()->insertIntoGrid(g, m);
	}

	if (gui)
	{
		glutInit(&argc, argv);
		GLCanvas canvas;
		canvas.initialize(sp, renderFunc, traceRayFunc, g, visualize_grid);
	}
	// OrthographicCamera *camera = (OrthographicCamera *)sp->getCamera();

	return 0;
}