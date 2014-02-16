#pragma once

#include "stdafx.h"

struct VertexObject
{
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 norm;
};


GLuint loadObjInVAO(const char* path);