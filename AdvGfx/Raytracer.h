#pragma once
#include <Windows.h>

#include "AdvGfx.h"
#include "PhotonMap.h"
#include <vector>
#include <random>

#include <glm\glm.hpp>
#include <glm\ext.hpp>

#define PI 3.14159265
#define e 2.71828

using namespace AdvGfxCore;

struct material
{
	glm::vec4 color;
	float refractivity,reflectivity, diffuse;
};


struct sphere
{
	material mat;
	glm::vec3 pos;
	float radius;
};

struct triangle
{
	material mat;
	glm::vec3 v0, v1, v2;
};

struct plane
{
	material mat;
	glm::vec3 normal, point;
};

struct light
{
	glm::vec3 location;
	glm::vec4 color;
	/*glm::vec3 dir;
	glm::vec3 normal;*/
	float intensity;
};

struct objects
{
	std::vector<sphere> spheres;
	std::vector<plane> planes;
	std::vector<light> lights;

	int nrPlanes, nrSpheres, nrLights;
};

struct photon
{
public:
	glm::vec3 position;
	glm::vec4 color;
	float intensity;	
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
	float intersect(ray *r, objects *scene, void** obj, int *type);
	glm::vec4 traceRay(ray* ray, objects* scene, int depth);
	pixel* shootRay(camera c);
	void emit(light l, objects* scene);
	void tracePhoton(photon f, glm::vec3 direction, light l,  objects* scene, int lvl);
	std::vector<photon> shootPhoton();

private:
	std::vector<photon> photonList;
	PhotonMap photonMap;
	int nrOfPhotons;
	std::random_device rd;
	std::mt19937 engine;

	glm::vec3 randomDirect();
};
objects createScene();