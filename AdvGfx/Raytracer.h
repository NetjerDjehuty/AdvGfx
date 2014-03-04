#pragma once
#include <Windows.h>

#include "AdvGfx.h"

#include <glm\glm.hpp>
#include <glm\ext.hpp>

using namespace AdvGfxCore;

struct objects
{
	sphere *spheres;
	plane *planes;

	int nrPlanes, nrSpheres;
};

struct ray
{
public:
	glm::vec3 origin, direction;
};

struct pixel
{
public:
	unsigned char r, g, b, a;
};

class RayTracer
{
public:
	RayTracer();
	~RayTracer();

	bool rayTriangleIntersection(triangle* t, ray* r);
	bool raySphereIntersection(sphere* s, ray* r, float* f);
	bool rayPlaneIntersection(plane* p, ray* r, float* f);
	pixel* shootRay(camera c);
};

objects createScene();