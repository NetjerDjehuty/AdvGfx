#pragma once

#include "stdafx.h"


struct Model
{
	GLuint vao;
	GLuint vbo[2];

	GLuint count;
};


Model loadObjInVAO(const char* path);