#pragma once

#include "ModelMesh.h"

#include <glm/glm.hpp>

#include <vector>


class Model
{
public:
	Model();
	~Model();

	glm::mat4 getModelMatrix() { return _mm; }
	void setModelMatrix(glm::mat4 modelMatrix) { _mm = modelMatrix; }

	void draw();
private:

	std::vector<ModelMesh> _modelParts;

	// Vertex Array Object
	unsigned int _vao[1];
	// Vertex Buffer Array
	unsigned int _vbo[2];

	// GL program ref
	unsigned int _prog;

	// Model Matrix
	glm::mat4 _mm;

	friend Model loadModel(const char* path);
};