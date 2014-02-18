#pragma once

#include<string>


class ModelMesh
{
public:
	ModelMesh(std::string material, unsigned int start, unsigned int count);
	~ModelMesh();

	const std::string& getMaterialRef() { return _matRef;};
	void draw();

private:
	// Reference to the material to be used for this mesh
	std::string _matRef;
	// Index into the vbo where this mesh starts
	unsigned int _startIndex;
	// Number of elements (triangles) in this model
	unsigned int _count;
};