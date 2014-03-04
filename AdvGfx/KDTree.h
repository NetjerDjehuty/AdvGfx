#pragma once

#include <vector>

#include <GLM\glm.hpp>
#include <GLM\ext.hpp>

class KDTree
{
public:
	glm::vec3 data1, data2, data3;

protected:

private:
public: 
	KDTree();
	~KDTree();

public:
	void constructTree(std::vector<glm::vec3> P, int depth);
	void search();
};

struct Leaf
{
	glm::vec3 data;
};

struct Node
{
	Node *lChild, *rChild;
};