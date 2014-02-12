#pragma once

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

namespace AdvGfxCore
{
	void Init();
	void Draw();
	unsigned int getFileLength(std::ifstream&);
	const char** constructArray(std::ifstream& file, int length);
}