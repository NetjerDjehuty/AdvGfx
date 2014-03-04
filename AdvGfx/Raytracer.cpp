#include "Raytracer.h"

using namespace AdvGfxCore;

RayTracer::RayTracer(){};
RayTracer::~RayTracer(){};

pixel *pixels;

bool RayTracer::rayTriangleIntersection(triangle* t, ray* r)
{
	// compute planes normal;
	glm::vec3 A, B;
	A = t->v1 - t->v0;
	B = t->v2 - t->v0;

	glm::vec3 N = glm::cross(A, B);

	// check if ray and plane are parrallel
	float nDotRay = glm::dot(N, r->direction);
	if(nDotRay == 0) // parrallel -> no intersection
		return false;
	float d = glm::dot(N, t->v0);
	float t_ = -(glm::dot(N, r->origin)+ d) / nDotRay;
	if(t_ < 0.0f) // triangle behind ray -> no intersection
		return false;

	glm::vec3 intersection = r->origin + t_ * r->direction;

	glm::vec3 C, edge0, edge1, edge2, vpo;
	edge0 = t->v1 - t->v0;
	vpo = intersection - t->v0;
	C = glm::cross(edge0, vpo);
	if(glm::dot(N,C) < 0)
		return false;

	edge1 = t->v2 - t->v1;
	vpo = intersection - t->v1;
	C = glm::cross(edge1, vpo);
	if(glm::dot(N,C) < 0)
		return false;

	edge2 = t->v0 - t->v2;
	vpo = intersection - t->v2;
	C = glm::cross(edge2, vpo);
	if(glm::dot(N,C) < 0)
		return false;

	return true;
};

bool RayTracer::raySphereIntersection(sphere* s, ray* r, float* f)
{
	glm::vec3 rayToCenter = (s->pos) - (r->origin);
	float dotProduct = glm::dot(r->direction, rayToCenter);
	float d = (dotProduct * dotProduct) - glm::dot(rayToCenter, rayToCenter) + (s->radius * s->radius);

	if(d < 0)
		return false;
	*f = (dotProduct - sqrt(d));
	if(*f < 0)
	{
		*f = (dotProduct + sqrt(d));
		if(*f < 0)
			return false;
	}
	return true;
};

bool RayTracer::rayPlaneIntersection(plane* p, ray* r, float* f)
{
	float dotProduct = glm::dot(r->direction, p->normal);
	if(dotProduct == 0)
		return false;
	*f = glm::dot(p->normal, p->point - r->origin) / dotProduct;

	return *f >= 0;
};

objects createScene()
{
	objects o = {};
	o.spheres = new sphere[1];
	o.planes = new plane[3];
	o.nrPlanes = 0, o.nrSpheres = 0;
	sphere s;
	s.pos = glm::vec3(1.f,1.f,1.f);
	s.radius = 3.0f;

	o.spheres[o.nrSpheres] = s;
	o.nrSpheres++;

	plane p;
	p.point = glm::vec3(10.f, 0.f, 1.f);
	p.normal = glm::vec3(1.f,0.f,0.f);

	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;

	p.point = glm::vec3(-10.f, 0.f, 1.f);
	p.normal = glm::vec3(-1.f, 0.f, 0.f);
	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;

	p.point = glm::vec3(0.f, 0.f, 0.f);
	p.normal = glm::vec3(0.f, 1.f, 0.f);
	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;

	return o;
}

pixel* RayTracer::shootRay(camera c)
{
	objects o = createScene();
	glm::mat4 viewProjectionMatrix = c.viewMatrix * c.projectionMatrix;

	ray r;
	glm::vec4 temp = c.viewMatrix * glm::vec4(0.f,0.f,-1.f,1.f);
	r.origin = glm::vec3(temp.x, temp.y, temp.z);

	//ax + by + cz + d = 0,
	float A = viewProjectionMatrix[0][2];
	float B = viewProjectionMatrix[1][2];
	float C = viewProjectionMatrix[2][2];
	float D = viewProjectionMatrix[3][2];

	RECT nearPlane = {};

	pixel p = {};
	p.a = 'a';
	p.b = 'b';
	p.r = 'r';
	p.g = 'g';

	const int width = 1280;
	const int height = 720;
	const float aspect = (float)width/(float)height;

	// Probleem kinderen
	pixel *pixels = new pixel[width*height];
	float *closestPoint = new float[width*height];
	// end probleem kinderen
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			float xx = (float) x / width + (1/width);
			float yy = (float) y / height + (1/height);

			xx = (xx - 0.5f) * aspect;
			yy = (yy - 0.5f);

			temp = c.viewMatrix * glm::vec4(xx, yy, 0, 1.0f);
			r.direction = glm::normalize(glm::vec3(temp.x, temp.y, temp.z) - r.origin);
			float f;
			closestPoint[y * width + x] = 90000.f;

			pixels[y * width + x].a = 0;
			pixels[y * width + x].r = 0;
			pixels[y * width + x].g = 0;
			pixels[y * width + x].b = 0;

			for(int i = 0; i < o.nrSpheres; i++)
			{
				sphere s = o.spheres[i];
				// Check if there is a collision
				if(RayTracer::raySphereIntersection(&s, &r, &f))
				{
					if(closestPoint[y * width + x] > f)
					{
						pixels[y * width + x].a = 255;
						pixels[y * width + x].r = 0;
						pixels[y * width + x].g = 255;
						pixels[y * width + x].b = 0;
						closestPoint[y * width + x] = f;
					}
				}
			}
			for(int i = 0; i < o.nrPlanes; i++)
			{
				plane p = o.planes[i];
				// Check if there is a collision
				if(RayTracer::rayPlaneIntersection(&p, &r, &f))
				{
					if(closestPoint[y * width + x] > f)
					{
						pixels[y * width + x].a = 255;
						pixels[y * width + x].r = 255;
						pixels[y * width + x].g = 0;
						pixels[y * width + x].b = 0;
						closestPoint[y * width + x] = f;
					}
				}
			}
			pixel p = pixels[y * width + x];
			/*if(p.g == 255 && p.a == 255)
				std::cout << 's';
			if(p.r == 255 && p.a == 255)
				std::cout << 'p';
			if(p.a == 0 && p.b == 0 && p.g == 0 && p.r == 0)
				std::cout << 'n';*/
		}
		//std::cout << '\n';
	}

	std::cout << "fertig" << std::endl;
	return pixels;
}

objects o;