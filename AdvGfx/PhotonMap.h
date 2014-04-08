#include <GLM\glm.hpp>

struct Photon
{
	glm::vec3 pos;
	short plane;
	unsigned char theta, phi;
	glm::vec3 power;
};

struct NearestPhoton
{
	int max;
	int found;
	int got_heap;
	glm::vec3 pos;
	float * dist2;
	const Photon **index;
};

class PhotonMap
{
public:
	PhotonMap(int capacity);
	~PhotonMap();

	void store(const glm::vec3 power, const glm::vec3 pos, const glm::vec3 dir);
	void scalePhotonPower(const float scale);
	void balance();
	void irradianceEstimate(glm::vec3 &irrad, const glm::vec3 pos, const glm::vec3 normal, const float maxDist, const int nrPhotons) const;
	void locatePhotons(NearestPhoton *const np, const int index = 1) const;
	void photonDir(glm::vec3 &dir, const Photon *p) const;

private:
	void balanceSeg(Photon **pbal, Photon **porg, const int index, const int start, const int end);
	void medianSplit(Photon **p, const int start, const int end, const int median, const int axis);

	Photon *photons;

	int storedPhotons, halfStoredPhotons, maxPhotons, prevScale;
	float cosTheta[256], sinTheta[256], cosPhi[256], sinPhi[256];
	glm::vec3 bboxMin, bboxMax;
};