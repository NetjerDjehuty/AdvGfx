// AdvGfx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AdvGfx.h"
#include "objLoader.h"
#include "Raytracer.h"

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

	GLuint loadProgram(const char* vertS, const char* fragS)
	{
		GLuint p;

		p = glCreateProgram();
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

		string v = textFileRead(vertS);
		string f = textFileRead(fragS);

		const char* vArr = v.c_str();
		const char* fArr = f.c_str();

		glShaderSource(vert, 1, &vArr, 0);
		glShaderSource(frag, 1, &fArr, 0);

		glCompileShaderARB(vert);
		glCompileShaderARB(frag);

		GLint compiled;

		glGetObjectParameterivARB(vert, GL_COMPILE_STATUS, &compiled);
		if (compiled)
		{
			glGetObjectParameterivARB(frag, GL_COMPILE_STATUS, &compiled);
			if (compiled)
			{
				//continue;
			}
			else
			{
				const unsigned int BUFFER_SIZE = 512;
				char buffer[BUFFER_SIZE];
				memset(buffer, 0, BUFFER_SIZE);
				GLsizei length = 0;

				glGetShaderInfoLog(frag, BUFFER_SIZE, &length, buffer);

				cout << buffer << endl;
			}
		}

		glAttachShader(p, frag);
		glAttachShader(p, vert);

		glLinkProgram(p);

		validateProgram(p);

		getErrors();

		return p;
	}

	glm::vec3 movement;

	GLuint prog;

	GLuint projLoc;
	GLuint viewLoc;
	GLuint modelLoc;

	GLuint posLoc;
	GLuint colLoc;
	GLuint powLoc;

	camera c;

	glm::mat4 modelMatrix;

	float xChange = 0, yChange = 0, zChange = 0;

	clock_t lastDraw = clock();

	Model* model = NULL;
	Model* model2 = NULL;

	GLuint texture;

	GLuint quadVAO;
	GLuint quadVBO;

	GLuint bbVAO[1];
	GLuint bbVBO[1];

	GLuint photonTex;

	void InitTex()
	{
		glGenTextures(1, &photonTex);
		glBindTexture(GL_TEXTURE_2D, photonTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		const int dim = 20;

		char *data = new char[dim*dim]; 

		float maxD = dim/2;

		for(int i = 0; i<dim; ++i)
		{
			for(int j = 0; j<dim; ++j)
			{
				float x = (i - (float)dim/2.f );
				float y = (j - (float)dim/2.f );

				float D = glm::length(glm::vec2(x,y));

				float dat = max(0.f,(maxD - D)/maxD * 255);

				data[i+j*dim] = (char)dat;
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, dim, dim, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		delete[] data;

		getErrors();
	}

	void Init(int w, int h)
	{
		//glClearColor(.1f, .2f, .3f, 1.f);
		glClearColor(0,0,0,0);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		InitTex();

		prog = loadProgram("bbShader.vert","bbShader.frag");


		projLoc = glGetUniformLocation(prog, "proj");
		viewLoc = glGetUniformLocation(prog, "view");
		posLoc = glGetUniformLocation(prog, "particleCenter_worldspace");
		colLoc = glGetUniformLocation(prog, "photonColor");
		powLoc = glGetUniformLocation(prog, "pow");
		/*modelLoc = glGetUniformLocation(prog, "model");*/

		ResetCamera();

		c.projectionMatrix = glm::perspective(60.0f, (float)w / h, 0.1f, 100.f);
		//glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), viewVec);
		/*modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));*/

		glUseProgram(prog);


		glUniformMatrix4fv(projLoc, 1, false, &c.projectionMatrix[0][0]);
		/*//glUniformMatrix4fv(viewLoc, 1, false, &viewMatrix[0][0]);
		glUniformMatrix4fv(modelLoc, 1, false, &modelMatrix[0][0]);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		*/
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f,
			1.0f, -1.0f,
			-1.0f,  1.0f,
			-1.0f,  1.0f,
			1.0f, -1.0f,
			1.0f,  1.0f,
		};

		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);

		glGenVertexArrays(1, &bbVAO[0]);
		glBindVertexArray(bbVAO[0]);


		static const GLfloat g_vertex_buffer_data[] = { 
			-0.5f, -0.5f,
			0.5f, -0.5f,
			-0.5f,  0.5f,
			-0.5f,  0.5f,
			0.5f, -0.5f,
			0.5f,  0.5f,
		};

		glGenBuffers(1, bbVBO);
		glBindBuffer(GL_ARRAY_BUFFER, bbVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		getErrors();
	}


	void DrawPhoton(const photon &p)
	{
		glUniform3f(posLoc, p.position.x, p.position.y, p.position.z);
		glUniform3f(colLoc, p.color.r, p.color.g, p.color.b);
		glUniform1f(powLoc, p.intensity);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	RayTracer r;

	void Draw()
	{
		//pixel *pixels = r.shootRay(c);

		std::vector<photon> photons = r.shootPhoton();

		clock_t startDraw = clock();

		// Draw the pixels

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);



		float duration = (startDraw - lastDraw) / (float)CLOCKS_PER_SEC;

		//viewVec += glm::vec3(xChange * duration, yChange * duration, zChange * duration);
		glm::vec3 finalMovement = 10 * duration * movement;
		finalMovement = glm::rotateX(finalMovement, -(c.xRot));
		finalMovement = glm::rotateY(finalMovement, -c.yRot);
		c.viewVec += finalMovement;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		c.viewMatrix = glm::mat4(1.0f);
		c.viewMatrix = glm::rotate(c.viewMatrix, c.xRot, glm::vec3(1.f, 0.f, 0.f));
		c.viewMatrix = glm::rotate(c.viewMatrix, c.yRot, glm::vec3(0.f, 1.f, 0.f));
		c.viewMatrix = glm::translate(c.viewMatrix, -(c.viewVec));

		//modelMatrix = glm::rotate(modelMatrix, (float)duration * 50, 0.f, 1.f, 0.f);

		glUniformMatrix4fv(viewLoc, 1, false, &c.viewMatrix[0][0]);
		//glUniformMatrix4fv(modelLoc, 1, false, &modelMatrix[0][0]);

		int count = photons.size();


		for(;count-->0;)
		{
			DrawPhoton(photons[count]);
		}

		getErrors();

		lastDraw = startDraw;

		xChange = yChange = zChange = 0;
	}

	void Resize(int width, int height)
	{

		glm::mat4 projectionMatrix = glm::perspective(60.0f, (float)width / height, 0.1f, 100.f);
		glUniformMatrix4fv(projLoc, 1, false, &projectionMatrix[0][0]);

		glViewport(0, 0, width, height);
	}



	void MoveCamera(float x, float y, float z)
	{
		xChange = x;
		yChange = y;
		zChange = z;
	}

	void MoveCamera(Movement m, bool stop)
	{
		switch (m)
		{
		case AdvGfxCore::Up:
			movement.y = (float)((int)!stop) * 1;
			break;
		case AdvGfxCore::Down:
			movement.y = (float)((int)!stop) * -1;
			break;
		case AdvGfxCore::Left:
			movement.x = (float)((int)!stop) * -1;
			break;
		case AdvGfxCore::Right:
			movement.x = (float)((int)!stop) * 1;
			break;
		case AdvGfxCore::Forward:
			movement.z = (float)((int)!stop) * -1;
			break;
		case AdvGfxCore::Backward:
			movement.z = (float)((int)!stop) * 1;
			break;			
		}
		std::cout << "viewvec: (" << c.viewVec.x << ',' << c.viewVec.y << ',' << c.viewVec.z << ") xRot: " << c.xRot <<  " yRot: " << c.yRot << std::endl;
	}

	void RotateCamera(int x, int y)
	{
		c.xRot -= y;
		c.yRot -= x;
	}

	void ResetCamera()
	{
		c.viewVec = glm::vec3(1.23f, -3.78f, -8.27f);
		c.xRot = -12.f;
		c.yRot = 180.f;
	}

	unsigned int getFileLength(ifstream& file)
	{
		if (!file.good()) return 0;
		int pos = file.tellg();
		file.seekg(0, ios::end);
		int len = file.tellg();
		file.seekg(ios::beg);

		cout << len << '\n';
		return len;
	}

	const char** constructArray(ifstream& file, int length)
	{
		char* arr = new char[length + 1];
		if (file.is_open())
		{
			int i = 0;
			while (file.good())
			{
				arr[i++] = file.get(); // gets char from line
			}
			arr[i] = 0;
		}
		file.close();

		const char* returnArray = arr;
		return &returnArray;
	}

	void load(const char* path)
	{
		if(model)
			delete model;

		model = loadModel(path, prog);
	}
}