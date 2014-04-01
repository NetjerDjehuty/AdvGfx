#include "PhotonMap.h"
#define PI 3.14159265
#define e 2.71828

PhotonMap :: PhotonMap(const int capacity)
{
	storedPhotons = 0;
	prevScale = 1;
	maxPhotons = capacity;

	photons = new Photon[capacity+1];

	bboxMin = glm::vec3(1e8f);
	bboxMin = glm::vec3(-1e8f);

	for(int i = 0; i <256; i++)
	{
		double angle = double(i) * (1.0/256.0)* PI;
		cosTheta[i] = cos(angle);
		cosPhi[i] = cos(2*angle);
		sinTheta[i] = sin(angle);
		sinPhi[i] = sin(2*angle);
	}
}

PhotonMap::~PhotonMap()
{
	delete[] photons;
}

void PhotonMap::photonDir(glm::vec3 dir, const Photon *p) const
{
	dir[0] = sinTheta[p->theta] * cosPhi[p->phi];
	dir[1] = sinTheta[p->theta] * sinPhi[p->phi];
	dir[2] = cosTheta[p->theta];
}

void PhotonMap::irradianceEstimate(glm::vec3 irrad, const glm::vec3 pos, const glm::vec3 normal, const float maxDist, const int nrPhotons) const
{
	irrad[0] = irrad[1] = irrad[2] = 0.0f;
	NearestPhoton np;
	np.dist2 = new float[nrPhotons+1];
	np.index = (const Photon**)(new Photon*[nrPhotons+1]);
	np.pos[0] = pos[0]; np.pos[1] = pos[1]; np.pos[2] = pos[2];
	np.max = nrPhotons;
	np.found = 0;
	np.got_heap = 0;
	np.dist2[0] = maxDist*maxDist;

	locatePhotons(&np, 1);

	if(np.found < 8)
		return;

	glm::vec3 pdir;

	for(int i = 1; i <= np.found; i++)
	{
		const Photon *p = np.index[i];
		photonDir(pdir, p);
		if(pdir[0] * normal[0] + pdir[1] * normal[1] + pdir[2] * normal[2] < 0.0f)
		{
			irrad[0] += p->power[0];
			irrad[1] += p->power[1];
			irrad[2] += p->power[2];
		}
	}

	const float temp = (1.0f/PI)/(np.dist2[0]);

	irrad *= temp;
}

void PhotonMap::locatePhotons(NearestPhoton *const np, const int index) const
{
	const Photon *p = &photons[index];
	float dist1;

	if(index < halfStoredPhotons)
	{
		dist1 = np->pos[p->plane] - p->pos[p->plane];

		if(dist1 > 0.0)
		{
			locatePhotons(np, 2*index+1);
			if(dist1 * dist1 < np->dist2[0])
				locatePhotons(np, 2*index);
		}
		else
		{
			locatePhotons(np, 2*index);
			if(dist1*dist1 < np->dist2[0])
				locatePhotons(np, 2*index+1);
		}
	}

	dist1 = p->pos[0] - np->pos[0];
	float dist2 = dist1*dist1;
	dist1 = p->pos[1] - np->pos[1];
	dist2 += dist1*dist1;
	dist1 = p->pos[2] - np->pos[2];
	dist2 += dist1*dist1;

	if(dist2 < np->dist2[0])
	{
		if(np->found < np->max)
		{
			np->found++;
			np->dist2[np->found] = dist2;
			np->index[np->found] = p;
		}
		else
		{
			int j,parent;
			if(np->got_heap==0)
			{
				float dst2;
				const Photon *phot;
				int halfFound = np->found>>1;
				for(int i = halfFound; i>=i; i--)
				{
					parent = i;
					phot = np->index[i];
					dst2 = np->dist2[i];
					while(parent <= halfFound)
					{
						j = parent+parent;
						if(j < np->found && np->dist2[j] < np->dist2[j+1])
							j++;
						if(dst2 >= np->dist2[j])
							break;
						np->dist2[parent] = np->dist2[j];
						np->index[parent] = np->index[j];
						parent = j;
					}
					np->dist2[parent] = dst2;
					np->index[parent] = phot;
				}
				np->got_heap = 1;
			}
			parent =1;
			j = 2;
			while(j <= np->found)
			{
				if(j < np->found && np->dist2[j] < np->dist2[j+1])
					j++;
				if(dist2 > np->dist2[j])
					break;
				np->dist2[parent] = np->dist2[j];
				np->index[parent] = np->index[j];
				parent = j;
				j += j;
			}
			np->index[parent] = p;
			np->dist2[parent] = dist2;
			np->dist2[0] = np->dist2[1];
		}
	}
}

void PhotonMap::store(const glm::vec3 power, const glm::vec3 pos, const glm::vec3 dir)
{
	if(storedPhotons > maxPhotons)
		return;

	storedPhotons++;
	Photon *const node = &photons[storedPhotons];

	for(int i=0; i<3; i++)
	{
		node->pos[i] = pos[i];
		if(node->pos[i] < bboxMin[i])
			bboxMin[i] = node->pos[i];
		if(node->pos[i] > bboxMax[i])
			bboxMax[i] = node->pos[i];
		node->power[i] = power[i];
	}

	int theta = (int) acos(dir[2])*(256.0/PI);
	if(theta > 255)
		node->theta = 255;
	else
		node->theta = theta;

	int phi = (int) atan2(dir[1], dir[0]) * (256.0/(2*PI));
	if(phi > 255)
		node->phi = 255;
	else if(phi < 0)
		node->phi = phi+256;
	else
		node->phi = phi;


}

void PhotonMap::scalePhotonPower(const float scale)
{
	for(int i = prevScale; i <= storedPhotons; i++)
	{
		photons[i].power *= scale;
	}
	prevScale = storedPhotons;
}

void PhotonMap::balance()
{
	if(storedPhotons>1)
	{
		Photon **pa1 = new Photon*[storedPhotons+1];
		Photon **pa2 = new Photon*[storedPhotons+1];

		for(int i =0; i <= storedPhotons; i++)
		{
			pa2[i] = &photons[i];
		}

		balanceSeg(pa1, pa2, 1, 1, storedPhotons);
		delete[] pa2;

		int d, j=1, k=1;
		Photon kPhoton = photons[j];

		for(int i = 1; i <= storedPhotons; i++)
		{
			d = pa1[j] - photons;
			pa1[j] = NULL;
			if(d != k)
				photons[j] = photons[d];
			else
				photons[j] = kPhoton;

			if(i < storedPhotons)
			{
				for(;k <= storedPhotons; k++)
					if(pa1[k] != NULL)
						break;
				kPhoton = photons[k];
				j = k;
			}
			continue;
		}
		delete[] pa1;
	}
	halfStoredPhotons = storedPhotons/2-1;
}

#define swap(ph, a, b) { Photon *ph2 = ph[a]; ph[a]=ph[b]; ph[b] = ph2;}

void PhotonMap::medianSplit(Photon **p, const int start, const int end, const int median, const int axis)
{
	int left = start, right = end;

	while(right > left)
	{
		const float v = p[right]->pos[axis];
		int i = left-1;
		int j = right;
		for(;;)
		{
			while(p[++i]->pos[axis] < v)
				;
			while(p[--j]->pos[axis] > v && j > left)
				;
			if(i >= j)
				break;
			swap(p, i, j);
		}

		swap(p, i, right);
		if(i >= median)
			right = i-1;
		if(i <= median)
			left = i+1;
	}
}

void PhotonMap::balanceSeg(Photon **pbal, Photon **porg, const int index, const int start, const int end)
{
	int median = 1;
	while((4*median) <= (end-start +1))
		median += median;
	if((3*median) <= (end-start+1))
	{
		median += median;
		median += start -1;
	}
	else
	{
		median = end-median+1;
	}

	int axis = 2;
	if(bboxMax[0] -bboxMin[0] > bboxMax[1] - bboxMin[1] &&
		bboxMax[0]-bboxMin[0] > bboxMax[2] - bboxMin[2])
		axis = 0;
	else if(bboxMax[1] -bboxMin[1] > bboxMax[2] - bboxMin[2])
		axis = 1;

	medianSplit(porg, start, end, median, axis);

	pbal[index] = porg[median];
	pbal[index]->plane = axis;

	if(median > start)
	{
		if(start < median-1)
		{
			const float temp = bboxMax[axis];
			bboxMax[axis] = pbal[index]->pos[axis];
			balanceSeg(pbal, porg, 2*index, start, median-1);
			bboxMax[axis] = temp;
		}
		else
			pbal[2*index] = porg[start];
	}

	if(median < end)
	{
		if(median+1 < end)
		{
			const float temp = bboxMin[axis];
			bboxMin[axis] = pbal[index]->pos[axis];
			balanceSeg(pbal, porg, 2*index+1, median+1, end);
			bboxMin[axis] = temp;
		}
		else
			pbal[2*index+1] = porg[end];
	}
}