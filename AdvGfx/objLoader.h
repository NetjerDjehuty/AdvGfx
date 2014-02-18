#pragma once

#include "stdafx.h"
#include <unordered_map>
#include "Material.h"

struct Model
{
	GLuint vao;
	GLuint vbo[2];

	GLuint count;
};


Model loadObjInVAO(const char* path);
std::unordered_map<std::string, Material> loadMaterial(const char* path);