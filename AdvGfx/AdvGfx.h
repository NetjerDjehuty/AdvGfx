#pragma once

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

namespace AdvGfxCore
{
	void Init(int width, int height);
	void Draw();
	void Resize(int width, int height);
	void MoveCamera(float x, float y, float z);
	void RotateCamera(int x, int y);
	void ResetCamera();
	unsigned int getFileLength(std::ifstream&);
	const char** constructArray(std::ifstream& file, int length);
}