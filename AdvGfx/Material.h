#pragma once

#include <glm\glm.hpp>

#include <string>
#include <unordered_map>

enum IllumModel : char
{
	Color = 0,
	ColorAmbient = 1,
	Hightlight = 2,
	ReflectRay = 3,
	GlassRay = 4,
	FresnelRay = 5,
	RefractRay = 6,
	RefractFresnelRay = 7,
	Reflect = 8,
	Glass = 9,
	CastsShadows = 10
};

class Material
{
public:
	void setMaterial(unsigned int prog);

	const std::string& getName() {return _name;}
private:
	std::string _name;

	glm::vec3 _ambColor;
	glm::vec3 _diffColor;
	glm::vec3 _specColor;
	glm::vec3 _transFilter;
	IllumModel _illum;
	float _trans;
	float _specPower;
	float _sharpness;
	float _optDensity;

	unsigned int _ambTex;
	unsigned int _diffTex;
	unsigned int _specColorTex;
	unsigned int _specPowerTex;
	unsigned int _transTex;
	unsigned int _decalTex;

	friend std::unordered_map<std::string, Material> loadMaterial(const char* path);
};