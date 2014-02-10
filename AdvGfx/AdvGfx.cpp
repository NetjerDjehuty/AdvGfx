// AdvGfx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AdvGfx.h"

namespace AdvGfxCore
{

	void init()
	{
		glClearColor(.1f, .2f, .3f, 1.f);
	}


	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}