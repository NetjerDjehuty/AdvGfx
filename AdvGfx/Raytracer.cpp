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
	o.nrPlanes = 0, o.nrSpheres = 0, o.nrLights = 0;
	sphere s;
	// First sphere (red)
	s.pos = glm::vec3(1.f,-2.f,30.f);
	s.radius = 3.0f;
	s.mat.color = glm::vec4(1,0,0,1);
	s.mat.reflectivity = 0.5f;
	s.mat.refractivity = 0.0f;
	s.mat.diffuse = 0.0f;

	o.spheres.push_back(s);
	o.nrSpheres++;

	// Second sphere (blue)
	s.pos = glm::vec3(-2.f,-5.f,20.f);
	s.radius = 1.5f;
	s.mat.color = glm::vec4(0,0,1,1);
	s.mat.reflectivity = 0.5f;
	s.mat.refractivity = 0.0f;
	s.mat.diffuse = 0.0f;

	o.spheres.push_back(s);
	o.nrSpheres++;

	plane p;
	// BOTTOM
	p.point = glm::vec3(0.f, -10.f, 0.f);
	p.normal = glm::vec3(0.f,1.f,0.f);
	p.mat.color = glm::vec4(1,0,1,1);
	p.mat.reflectivity = 0.5f;
	p.mat.refractivity = 0.0f;
	p.mat.diffuse = 0.0f;
	o.planes.push_back(p);
	o.nrPlanes++;

	// TOP
	p.point = glm::vec3(0.f, 5.f, 0.f);
	p.normal = glm::vec3(0.f, -1.f, 0.f);
	p.mat.color = glm::vec4(1,1,1,1);
	p.mat.reflectivity = 0.5f;
	p.mat.refractivity = 0.0f;
	p.mat.diffuse = 0.01f;
	o.planes.push_back(p);
	o.nrPlanes++;

	// RIGHT
	p.point = glm::vec3(-10.f, -5.f, 0.f);
	p.normal = glm::vec3(1.f, 0.f, 0.f);
	p.mat.color = glm::vec4(1,0,0,1);
	p.mat.reflectivity = 0.0f;
	p.mat.refractivity = 0.0f;
	p.mat.diffuse = 0.0f;
	o.planes.push_back(p);
	o.nrPlanes++;

	// LEFT
	p.point = glm::vec3(10.f, -5.f, 0.f);
	p.normal = glm::vec3(-1.f, 0.f, 0.f);
	p.mat.color = glm::vec4(0,1,0,1);
	p.mat.reflectivity = 0.4f;
	p.mat.refractivity = 0.0f;
	p.mat.diffuse = 0.4f;
	o.planes.push_back(p);
	o.nrPlanes++;

	/// BACK
	p.point = glm::vec3(0.f, 0.f, 40.f);
	p.normal = glm::vec3(0.f, 0.f, -1.f);
	p.mat.color = glm::vec4(0,0,1,1);
	p.mat.reflectivity = 0.0f;
	p.mat.refractivity = 0.0f;
	p.mat.diffuse = 0.0f;
	o.planes.push_back(p);
	o.nrPlanes++;

	light l;

	l.color = glm::vec4((float)1/3);
	l.intensity = 5000;
	l.location = glm::vec3(-8,4.5,38);
	l.dir = glm::normalize(l.location - s.pos);
	if(l.dir.x <= l.dir.y && l.dir.x <= l.dir.z)
		l.normal = glm::normalize(glm::vec3((-1)*l.dir.x, 0.0f, 0.0f));
	if(l.dir.y < l.dir.x && l.dir.y < l.dir.z)
		l.normal = glm::normalize(glm::vec3(0.0f,(-1)*l.dir.y, 0.0f));
	if(l.dir.z < l.dir.x && l.dir.z < l.dir.y)
		l.normal = glm::normalize(glm::vec3(0.0f, 0.0f,(-1)*l.dir.z));
	o.lights.push_back(l);
	o.nrLights++;

	l.color = glm::vec4((float)1/3);
	l.intensity = 5000;
	l.location = glm::vec3(6,4,20);
	l.dir = glm::normalize(l.location - s.pos);
	if(l.dir.x <= l.dir.y && l.dir.x <= l.dir.z)
		l.normal = glm::normalize(glm::vec3((-1)*l.dir.x, 0.0f, 0.0f));
	if(l.dir.y < l.dir.x && l.dir.y < l.dir.z)
		l.normal = glm::normalize(glm::vec3(0.0f,(-1)*l.dir.y, 0.0f));
	if(l.dir.z < l.dir.x && l.dir.z < l.dir.y)
		l.normal = glm::normalize(glm::vec3(0.0f, 0.0f,(-1)*l.dir.z));
	o.lights.push_back(l);
	o.nrLights++;

	l.color = glm::vec4((float)1/3);
	l.intensity = 5000;
	l.location = glm::vec3(2,3,8);
	l.dir = glm::normalize(l.location - s.pos);
	if(l.dir.x < l.dir.y && l.dir.x < l.dir.z)
		l.normal = glm::normalize(glm::vec3((-1)*l.dir.x, 0.0f, 0.0f));
	if(l.dir.y < l.dir.x && l.dir.y < l.dir.z)
		l.normal = glm::normalize(glm::vec3(0.0f,(-1)*l.dir.y, 0.0f));
	if(l.dir.z < l.dir.x && l.dir.z < l.dir.y)
		l.normal = glm::normalize(glm::vec3(0.0f, 0.0f,(-1)*l.dir.z));
	o.lights.push_back(l);
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

glm::vec3 reflect(glm::vec3 V, glm::vec3 N){
	return V + (2.0f * N * -glm::dot(V, N));
}

glm::vec3 refract(glm::vec3 V, glm::vec3 N, float refrIndex)
{
	float cosI = -glm::dot( N, V );
	float cosT2 = 1.0f - refrIndex * refrIndex * (1.0f - cosI * cosI);
	return (refrIndex * V) + (refrIndex * cosI - sqrt( cosT2 )) * N;
}

glm::vec4 RayTracer::traceRay(ray* r, objects* scene, int depth)
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

		glm::vec4 reflectColor = m.color, refractColor = m.color;
		if(depth <= 4)
		{
			if (m.reflectivity > 0 )
			{
				ray reflectRay;
				glm::vec3 R = reflect(r->direction, normal);
				reflectRay.origin = intersectPos + R*0.001f;
				reflectRay.direction = R;
				reflectColor = m.reflectivity * traceRay(&reflectRay, scene, depth + 1);
			} 
			else if (m.refractivity > 0 )
			{
				ray refractRay;
				glm::vec3 R = refract(r->direction, normal, 0.6);
				if (glm::dot(R,normal) < 0 ){
					refractRay.origin = intersectPos + R*0.001f;
					refractRay.direction = R;
					refractColor = m.refractivity * traceRay(&refractRay, scene, depth +1);
				}
			}
		}

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

			glm::vec4 diffuseColor = refractColor + reflectColor + m.color * (m.reflectivity + m.refractivity);

			color += pointLit* diffuseColor *scene->lights[i].color*glm::max(0.0f,glm::dot(normal, L));
		}
	}
	return glm::clamp(color, 0.0f, 1.0f) * 255.0f;
}



pixel* RayTracer::shootRay(camera c)
{
	objects o = createScene();
	/*shootPhoton(&o);/*
	glm::mat4 viewProjectionMatrix = c.viewMatrix * c.projectionMatrix;

	ray r;
	glm::vec4 temp = c.viewMatrix * glm::vec4(0.f,0.f,-1.f,1.f);
	r.origin = glm::vec3(temp.x, temp.y, temp.z);

	pixel p = { 0,0,0,255 };

	const int width = 1280;
	const int height = 720;
	const float aspect = (float)width/(float)height;

	pixel *pixels = new pixel[width*height];
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

	glm::vec4 result = RayTracer::traceRay(&r,&o, 1);
	pixels[y * width + x].a = result.a;
	pixels[y * width + x].r = result.r;
	pixels[y * width + x].g = result.g;
	pixels[y * width + x].b = result.b;

	}
	}*/

	return pixels;
}

glm::vec3 randomDirect()
{
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real<float> dist(-1.0f, 1.0f);

	float x = dist(engine), y = dist(engine), z = dist(engine);
	return glm::vec3(x,y,z);
}

int emitted = 0, oldemitted = 0, run = 1;

// Traces a photon through the scene and going in recursion if needed
// if not it stores the photon in the photon map with its new collision position
void RayTracer::tracePhoton(photon f, glm::vec3 direction, light l, objects* scene)
{
	float ksi = ((float) rand() / (RAND_MAX));
	void* intersectObj = 0;
	int intersectObjType = 0;

	// Need ray to quickly determine intersection point
	ray r;
	r.origin = f.position; // so the position and direction are stored in a new ray
	r.direction = direction;

	float t = intersect(&r, scene, &intersectObj, &intersectObjType);
	if (t > 0 && t < maxDist ) // Intersection before the max redering distance is achieved
	{		// If it doesn't collide before, shits going down :(
		glm::vec3 normal;
		f.position = f.position + direction * t;

		material m;

		if ( intersectObjType == 1 ){		
			normal = glm::normalize(f.position-((sphere*)intersectObj)->pos);
			m = ((sphere*)intersectObj)->mat;
		}
		else if (intersectObjType == 2 ){
			normal = ((plane*)intersectObj)->normal;
			m = ((plane*)intersectObj)->mat;
		}

		if(f.color == glm::vec4(0)) // not reflected
			f.color = (l.color * m.color);
		else // reflected
			f.color += (l.color * m.color);
		f.color.a = 1;

		float s = m.reflectivity;
		float d = m.diffuse;


		glm::vec3 newDir;
		if(0 < ksi && ksi < d) // diffuse reflection
		{
			float brdf = glm::dot(glm::normalize(direction), normal) /** m.color * f.intensity*/;
			newDir = randomDirect();
			float propDens = (1 / sqrt(2*PI)) * pow(e, (pow(-newDir.x, 2)/2));
			const int c = 5;
			float newRandom = (1/ sqrt(2*PI)) * pow(e, (pow(-(brdf-c), 2)/2));

			while(newDir.x * newDir.x + newDir.y * newDir.y + newDir.z *newDir.z > 1)
			{
				newDir = randomDirect();
			}
			newDir = reflect(glm::normalize(direction), normal);

			tracePhoton(f, newDir, l, scene);
		}
		else if(ksi > d && ksi < (s+d)) // specular reflection
		{
			newDir = reflect(glm::normalize(direction), normal);
			tracePhoton(f, newDir, l, scene);
		}
		else // absorbed
		{
			photonMap.push_back(f);
			emitted++;
		}
	}
}

void RayTracer::emit(light l, objects* o)
{
	photon f;
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real<float> dist(-1.0f, 1.0f);
	while(emitted < nrOfPhotons * run)
	{
		float x = dist(engine), y = dist(engine), z = dist(engine);
		while((x*x) + (y*y) + (z*z) > 1)
		{
			x = dist(engine), y = dist(engine), z = dist(engine);
		}

		glm::vec3 direction = glm::normalize(glm::vec3(x,y,z));
		f.position = l.location;
		tracePhoton(f, direction, l, o);
	}
	for(int i = oldemitted; i < emitted; i++)
	{
		photonMap[i].intensity = (l.intensity/(emitted-oldemitted));
	}
	oldemitted = emitted;
	run++;
}


std::vector<photon> RayTracer::shootPhoton()
{
	if(photonMap.size() >0 )
		return photonMap;

	objects scene = createScene();

	nrOfPhotons = 5000;
	photon f;

	for(int i = 0; i < scene.nrLights; i++)
	{
		light l = scene.lights[i];
		emit(l, &scene);
	}

	return photonMap;
}
objects o;