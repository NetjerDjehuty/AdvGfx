#include "Model.h"

#include "stdafx.h"

Model::Model() : _currMat(""), _prog(0), _mm(glm::mat4(1.0f))
{
	glGenVertexArrays(1, _vao);
	glBindVertexArray(_vao[0]);
	glGenBuffers(2, _vbo);

	glBindVertexArray(NULL);
}


Model::~Model()
{
	glDeleteBuffers(2, _vbo);
	glDeleteVertexArrays(1, _vbo);
}

void Model::draw()
{
	glBindVertexArray(_vao[0]);

	for (int i = 0; i < _modelParts.size(); ++i)
	{
		std::string mat = _modelParts[i].getMaterialRef();
		if(_currMat != mat)
		{
			_materials[mat].setMaterial(_prog);
			_currMat = mat;
		}

		_modelParts[i].draw();
	}

	glBindVertexArray(NULL);
	_currMat = "";
}