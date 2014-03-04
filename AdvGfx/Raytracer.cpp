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
	o.planes = new plane[5];
	o.lights = new light[3];
	o.nrPlanes = 0, o.nrSpheres = 0, o.nrLights = 0;
	sphere s;
	s.pos = glm::vec3(1.f,-2.f,30.f);
	s.radius = 3.0f;
	s.mat.color = glm::vec4(1,0,0,1);

	o.spheres[o.nrSpheres] = s;
	o.nrSpheres++;

	plane p;
	p.point = glm::vec3(0.f, -10.f, 0.f);
	p.normal = glm::vec3(0.f,1.f,0.f);
	p.mat.color = glm::vec4(1,1,1,1);
	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;

	p.point = glm::vec3(0.f, 5.f, 0.f);
	p.normal = glm::vec3(0.f, -1.f, 0.f);
	p.mat.color = glm::vec4(1,1,1,1);
	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;

	p.point = glm::vec3(-10.f, -5.f, 0.f);
	p.normal = glm::vec3(1.f, 0.f, 0.f);
	p.mat.color = glm::vec4(1,0,0,1);
	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;
	
	p.point = glm::vec3(10.f, -5.f, 0.f);
	p.normal = glm::vec3(-1.f, 0.f, 0.f);
	p.mat.color = glm::vec4(0,1,0,1);
	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;

	p.point = glm::vec3(0.f, 0.f, 40.f);
	p.normal = glm::vec3(0.f, 0.f, -1.f);
	p.mat.color = glm::vec4(1,1,1,1);
	o.planes[o.nrPlanes] = p;
	o.nrPlanes++;

	light l;
	l.color = glm::vec4(0,0,1,1);
	l.location = glm::vec3(-8,4.5,38);
	l.dir = glm::normalize(l.location - s.pos);
	o.lights[o.nrLights] = l;
	o.nrLights++;
	
	l.color = glm::vec4(0,1,0,1);
	l.location = glm::vec3(6,4,20);
	l.dir = glm::normalize(l.location - s.pos);
	o.lights[o.nrLights] = l;
	o.nrLights++;
	
	l.color = glm::vec4(1,0,0,1);
	l.location = glm::vec3(2,3,35);
	l.dir = glm::normalize(l.location - s.pos);
	o.lights[o.nrLights] = l;
	o.nrLights++;

	return o;
}

const float maxDist = 90000.f;

float RayTracer::intersect(ray *r, objects *o, void** obj, int*type)
{
	float f = maxDist;
	float closestPoint = maxDist;

	for(int i = 0; i < o->nrSpheres; i++)
	{
		sphere s = o->spheres[i];
		// Check if there is a collision
		if(RayTracer::raySphereIntersection(&s, r, &f))
		{
			if(closestPoint > f)
			{
				closestPoint = f;
				*type = 1;
				*obj = (void*)&o->spheres[i];
			}
		}
	}
	for(int i = 0; i < o->nrPlanes; i++)
	{
		plane p = o->planes[i];
		// Check if there is a collision
		if(RayTracer::rayPlaneIntersection(&p, r, &f))
		{
			if(closestPoint > f)
			{
				closestPoint = f;
				*type = 2;
				*obj = (void*)&o->planes[i];
			}
		}
	}

	return closestPoint;
}

glm::vec4 RayTracer::traceRay(ray* r, objects* scene )
{
	void* intersectObj = 0;
	int intersectObjType = 0;
	float t = intersect( r, scene, &intersectObj, &intersectObjType);

	glm::vec4 color(0);	
	if ( t < maxDist ){		
		glm::vec3 intersectPos = r->origin+r->direction*t ;
		glm::vec3 normal;

		material m;

		if ( intersectObjType == 1 ){		
			normal = glm::normalize(intersectPos-((sphere*)intersectObj)->pos);
			m = ((sphere*)intersectObj)->mat;
		}
		else if (intersectObjType == 2 ){
			normal = ((plane*)intersectObj)->normal;
			m = ((plane*)intersectObj)->mat;
		}


		glm::vec4 diffuseColor = m.color;

		/*if (m.reflectivity > 0 )
		{
		ray reflectRay;
		float3 R = reflect(ray->dir, normal);
		reflectRay.origin = intersectPos + R*0.001;
		reflectRay.dir    = R;
		diffuseColor += m.reflectivity*raytrace2(&reflectRay, scene);
		} 
		else if (m.refractivity > 0 )
		{
		struct Ray refractRay;
		float3 R = refract(ray->dir, normal, 0.6);
		if ( dot(R,normal) < 0 ){
		refractRay.origin = intersectPos + R*0.001;
		refractRay.dir    = R;
		diffuseColor += m.refractivity*raytrace2(&refractRay, scene);
		}
		}*/

		for(int i = 0; i < scene->nrLights; i++){
			float lightDist = glm::length(scene->lights[i].location - intersectPos);
			glm::vec3 L = scene->lights[i].dir;
			L = scene->lights[i].location - intersectPos ;
			lightDist = glm::length(L);
			L = glm::normalize(L);

			float pointLit = 1;
			ray shadowRay;
			shadowRay.origin = intersectPos + L*0.001f;
			shadowRay.direction = L;
			t = intersect( &shadowRay, scene, &intersectObj, &intersectObjType);
			if ( t < lightDist ){
				pointLit = 0;
			}
			color += pointLit*diffuseColor*scene->lights[i].color*glm::max(0.0f,glm::dot(normal, L));
		}
	}
	return glm::clamp(color,0.0f,1.0f) * 255;
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

	pixel p = { 0,0,0,255 };

	const int width = 1280;
	const int height = 720;
	const float aspect = (float)width/(float)height;

	// Probleem kinderen
	pixel *pixels = new pixel[width*height];
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

			pixels[y * width + x] = p;

			glm::vec4 result = RayTracer::traceRay(&r,&o);
			pixels[y * width + x].a = result.a;
			pixels[y * width + x].r = result.r;
			pixels[y * width + x].g = result.g;
			pixels[y * width + x].b = result.b;

		}
		//std::cout << '\n';
	}

	std::cout << "fertig" << std::endl;
	return pixels;
}

objects o;