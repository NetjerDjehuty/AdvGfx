// AdvGfx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AdvGfx.h"
#include <fstream>
#include <iostream>
#include <string>


namespace AdvGfxCore
{
	using namespace std;
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

		GLuint prog = glCreateProgram();
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

		unsigned int vlength = getFileLength(ifstream("basic.vert"));
		unsigned int flength = getFileLength(ifstream("basic.frag"));

		GLint vLength = (GLint) vlength;
		GLint fLength = (GLint) flength;

		const char** vArr = constructArray(ifstream("basic.vert"), vlength);
		const char** fArr = constructArray(ifstream("basic.frag"), flength);

		glShaderSource(vert, 1, vArr, &vLength);
		glShaderSource(frag, 1, fArr, &fLength);
		//glShaderSource(frag, 1, flength, fArr);

		glCompileShaderARB(vert);
		glCompileShaderARB(frag);

		GLint compiled;

		glGetObjectParameterivARB(vert, GL_COMPILE_STATUS, &compiled);
		if(compiled)
		{
			glGetObjectParameterivARB(frag, GL_COMPILE_STATUS, &compiled);
			if(compiled)
			{
				//continue;
			}
		}

		glAttachShader(prog, frag);
		glAttachShader(prog, vert);

		glLinkProgram(prog);

		glGenBuffers(1, &posBuffObj);

		glBindBuffer(GL_ARRAY_BUFFER, posBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//delete[] vArr;
		//delete[] fArr;
	}


	void Draw()
	{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBindBuffer(GL_ARRAY_BUFFER, posBuffObj);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}

	unsigned int getFileLength(ifstream& file)
	{
		if(!file.good()) return 0;    
		unsigned int pos=file.tellg();
		file.seekg(0,ios::end);
		unsigned int len = file.tellg();
		file.seekg(ios::beg);
    
		cout << len << '\n';
		return len;
	}

	const char** constructArray(ifstream& file, int length)
	{
		char* arr = new char[length+1];
		if(file.is_open())
		{
			int i = 0;
			while(file.good())
			{
				arr[i] = file.get(); // gets char from line
			}
			arr[i] = 0;
		}
		file.close();

		const char* returnArray = arr;
		return &returnArray;
	}
}