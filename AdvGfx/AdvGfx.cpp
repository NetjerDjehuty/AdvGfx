// AdvGfx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AdvGfx.h"

namespace AdvGfxCore
{

	GLuint posBuffObj;

	void Init()
	{
		glClearColor(.1f, .2f, .3f, 1.f);


		// simple triangle
		const float vertexPositions[] = {
			0.75f, 0.75f, 0.0f, 1.0f,
			0.75f, -0.75f, 0.0f, 1.0f,
			-0.75f, -0.75f, 0.0f, 1.0f,
		};

		glGenBuffers(1, &posBuffObj);

		glBindBuffer(GL_ARRAY_BUFFER, posBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void Draw()
	{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBindBuffer(GL_ARRAY_BUFFER, posBuffObj);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}
}