#pragma once

#include <glm/glm.hpp>

#include <vector>

class ModelMesh
{
public:
	ModelMesh(unsigned int material, unsigned int start, unsigned int count);
	~ModelMesh();

	void draw();

private:
	// Reference to the material to be used for this mesh
	unsigned int _matRef;
	// Index into the vbo where this mesh starts
	unsigned int _startIndex;
	// Number of elements (triangles) in this model
	unsigned int _count;
};

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