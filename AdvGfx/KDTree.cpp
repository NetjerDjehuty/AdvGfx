// AdvGfx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KDTree.h"

KDTree::KDTree(){}
KDTree::~KDTree(){}

void KDTree::constructTree(std::vector<glm::vec3> P, int depth)
{
	if(P.size() == 1)
	{
		
	}
}

KDTree kdt;