#pragma once

#include "stdafx.h"
#include <unordered_map>
#include "Material.h"
#include "Model.h"

Model* loadModel(const char* path);
std::unordered_map<std::string, Material> loadMaterial(const char* path);