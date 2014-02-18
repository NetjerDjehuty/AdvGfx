#pragma once


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