// AdvGfx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AdvGfx.h"
#include "objLoader.h"

#include <fstream>
#include <iostream>
#include <string>

#include <GLM\glm.hpp>
#include <GLM\ext.hpp>



namespace AdvGfxCore
{

	using namespace std;

	void getErrors()
	{
		GLenum error = glGetError();
		while (error)
		{
			cout << gluErrorString(error) << endl;
			error = glGetError();
		}
	}


	static string textFileRead(const char *fileName) {
		string fileString = string(); // A string for storing the file contents
		string line = string(); // A string for holding the current line

		ifstream file(fileName); // Open an input stream with the selected file
		if (file.is_open()) { // If the file opened successfully
			while (!file.eof()) { // While we are not at the end of the file
				getline(file, line); // Get the current line
				fileString.append(line); // Append the line to our file string
				fileString.append("\n"); // Appand a new line character
			}
			file.close(); // Close the file
		}

		return fileString; // Return our string
	}

	static bool validateProgram(GLuint program) {
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;

		glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer); // Ask OpenGL to give us the log associated with the program
		if (length > 0) // If we have any information to display
		{
			cout << "Program " << program << " link error: " << buffer << endl; // Output the information
			return false;
		}

		glValidateProgram(program); // Get OpenGL to try validating the program
		GLint status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status); // Find out if the shader program validated correctly
		if (status == GL_FALSE) // If there was a problem validating
		{
			cout << "Error validating shader " << program << endl; // Output which program had the error
			return false;
		}

		return true;
	}

	GLuint vao;
	GLuint posBuffObj;
	GLuint prog;
	
	GLuint viewLoc;
	glm::vec3 viewVec;
	float xRot = 0.0f;
	float yRot = 0.0f;

	void Init()
	{
		GLuint vao = loadObjInVAO("object.obj");

		glClearColor(.1f, .2f, .3f, 1.f);


		// simple triangle
		/*const float vertexPositions[] = {
			0.75f, 0.75f, 0.0f,
			0.75f, -0.75f, 0.75f,
			-0.75f, -0.75f, 0.75f,
		};*/

		prog = glCreateProgram();
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

		//unsigned int vlength = getFileLength(ifstream("basic.vert"));
		//unsigned int flength = getFileLength(ifstream("basic.frag"));

		//GLint vLength = (GLint) vlength;
		//GLint fLength = (GLint) flength;

		//const char** vArr = constructArray(ifstream("basic.vert"), vlength);
		//const char** fArr = constructArray(ifstream("basic.frag"), flength);

		string v = textFileRead("basic.vert");
		string f = textFileRead("basic.frag");

		const char* vArr = v.c_str();
		const char* fArr = f.c_str();

		glShaderSource(vert, 1, &vArr, 0);
		glShaderSource(frag, 1, &fArr, 0);
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

		validateProgram(prog);
		
		/*glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &posBuffObj);
		glBindBuffer(GL_ARRAY_BUFFER, posBuffObj);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		*/
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		int projectionMatrixLocation = glGetUniformLocation(prog, "projection");
		viewLoc = glGetUniformLocation(prog, "view");
		int modelMatrixLocation = glGetUniformLocation(prog, "model");

		viewVec = glm::vec3(0.0f, 0.0f, 5.0f);

		glm::mat4 projectionMatrix = glm::perspective(60.0f, 4.f / 3.f, 0.1f, 100.f);  
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), viewVec);
		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		glUseProgram(prog);


		glUniformMatrix4fv(projectionMatrixLocation, 1, false, &projectionMatrix[0][0]);
		glUniformMatrix4fv(viewLoc, 1, false, &viewMatrix[0][0]);
		glUniformMatrix4fv(modelMatrixLocation, 1, false, &modelMatrix[0][0]);


		getErrors();
	}


	void Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::rotate(viewMatrix, yRot, 1.f, 0.f, 0.f);
		viewMatrix = glm::rotate(viewMatrix, xRot, 0.f, 1.f, 0.f);
		viewMatrix = glm::translate(viewMatrix, -viewVec);

		glUniformMatrix4fv(viewLoc, 1, false, &viewMatrix[0][0]);
		
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		getErrors();
	}

	void MoveCamera(int x, int y, int z)
	{
		//int viewMatrixLocation = glGetUniformLocation(prog, "view");
		viewVec += glm::vec3(x,y,z);
		//viewMatrix = glm::translate(glm::mat4(1.0f), viewVec);
		//viewMatrix = glm::rotate(viewMatrix, degree, glm::normalize(viewVec));
		//glUniformMatrix4fv(viewMatrixLocation, 1, false, &viewMatrix[0][0]);
	}
	
	void RotateCamera(int x, int y)
	{
		/*if(x < 0)
			degree += x;
		else
			degree += x;*/
		xRot -= x;
		yRot -= y;
		//int viewMatrixLocation = glGetUniformLocation(prog, "view");
		//viewMatrix = glm::translate(glm::mat4(1.f), viewVec);
		//viewMatrix = glm::rotate(viewMatrix, xRot, glm::vec3(0.f,1.f,0.f));
		//glUniformMatrix4fv(viewMatrixLocation, 1, false, &viewMatrix[0][0]);
	}

	void ResetCamera()
	{
		//int viewMatrixLocation = glGetUniformLocation(prog, "view");
		viewVec = glm::vec3(0.0f, 0.0f, 5.0f);
		xRot = 0.f;
		yRot = 0.f;
		//viewMatrix = glm::translate(glm::mat4(1.0f), viewVec);
		//glUniformMatrix4fv(viewMatrixLocation, 1, false, &viewMatrix[0][0]);
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
				arr[i++] = file.get(); // gets char from line
			}
			arr[i] = 0;
		}
		file.close();

		const char* returnArray = arr;
		return &returnArray;
	}
}