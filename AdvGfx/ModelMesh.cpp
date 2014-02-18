#include "ModelMesh.h"

#include "stdafx.h"

ModelMesh::ModelMesh(std::string material, unsigned int start, unsigned int count)
	: _matRef(material), _startIndex(start), _count(count)
{}

ModelMesh::~ModelMesh()
{}

void ModelMesh::draw()
{
	glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, (GLvoid*)_startIndex);
}