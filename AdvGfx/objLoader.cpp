#include "objLoader.h"

#include <string>
#include <vector>
#include <map>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm\glm.hpp>

using namespace std;



GLuint loadObjInVAO(const char* path)
{
	// temporary storage for positions, tex coords and normals
	vector<glm::vec3> pos_t;
	vector<glm::vec2> tex_t;
	vector<glm::vec3> norm_t;

	// map for storing temporary indices
	map<string, GLuint> ind_map;

	// indexed storage
	vector<glm::vec3> pos;
	vector<glm::vec2> tex;
	vector<glm::vec3> norm;
	vector<GLuint> indices;

	ifstream file(path);
	if (!file.is_open())
	{
		cout << "Could not open file: " << path << endl;
		return NULL;
	}


	string line;

	while (!file.eof())
	{
		getline(file, line);
		istringstream lineStream(line);

		string token;
		lineStream >> token;

		if (token == "v")
		{
			float x, y, z;
			lineStream >> x >> y >> z;
			glm::vec3 pos(x, y, z);

			pos_t.push_back(pos);
			//cout << "Vertex: " << x << ',' << y << ',' << z << endl;
		}
		else if (token == "vt")
		{
			float u, v;
			lineStream >> u >> v;
			glm::vec2 pos(u, v);

			tex_t.push_back(pos);
			//cout << "TexCoord: " << u << ',' << v << endl;
		}
		else if (token == "vn")
		{
			float x, y, z;
			lineStream >> x >> y >> z;
			glm::vec3 norm(x, y, z);

			norm_t.push_back(norm);
			//cout << "Normal: " << x << ',' << y << ',' << z << endl;
		}
		else if (token == "f")
		{
			string set[5];
			lineStream >> set[0] >> set[1] >> set[2] >> set[3] >> set[4];

			if (set[4].size() > 0)
			{
				// N-sided polygon, won't support
				cout << "N-Sided polygons are not suported!" << endl;
				break;
			}
			if (set[3].size() > 0)
			{
				// Support for quads will come when we need it
				cout << "Quads are not supported (yet)!" << endl;
				break;
			}

			//cout << "Faces: " << set[0] << ',' << set[1] << ',' << set[2] <<  endl;
			for (int i = 0; i < 3; ++i)
			{
				if (ind_map.count(set[i]))
				{
					indices.push_back(ind_map[set[i]]);
				}
				else
				{
					unsigned int i1, i2, i3;

				}
			}
		}
	}
	file.close();

	return NULL;
}