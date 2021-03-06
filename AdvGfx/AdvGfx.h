#pragma once

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

namespace AdvGfxCore
{
	enum Movement
	{
		Up,
		Down,
		Left,
		Right,
		Forward,
		Backward
	};

	struct camera
	{
		// Camera positie
		glm::vec3 viewVec;

		// Richting
		float xRot;
		float yRot;

		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
	};


	void Init(int width, int height);
	void Draw();
	void Resize(int width, int height);
	void MoveCamera(float x, float y, float z);
	void MoveCamera(Movement m, bool stop);
	void RotateCamera(int x, int y);
	void ResetCamera();
	unsigned int getFileLength(std::ifstream&);
	const char** constructArray(std::ifstream& file, int length);
	void load(const char* path);
}