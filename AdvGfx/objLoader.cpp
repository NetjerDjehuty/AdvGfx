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
	vector<VertexObject> buffer;
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
					cout << "Face: " << set[i] << " found in map" << endl;
				}
				else
				{
					unsigned int i1, i2, i3;
					int matches = sscanf_s(set[i].c_str(), "%d/%d/%d", &i1, &i2, &i3);

					VertexObject vert = { pos_t[i1 - 1], tex_t[i2 - 1], norm_t[i3 - 1] };

					buffer.push_back(vert);

					GLuint index = buffer.size() - 1;
					ind_map[set[1]] = index;

					indices.push_back(index);
					cout << "Face: " << set[i] << " added at index " << index << endl;
				}
			}
		}
	}
	file.close();

	GLuint vao;
	GLuint vbo[2];

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexObject) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(NULL);

	return vao;
}