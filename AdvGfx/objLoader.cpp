#include "objLoader.h"

#include <string>
#include <vector>
#include <map>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm\glm.hpp>

using namespace std;

struct VertexObject
{
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 norm;
};

Model loadObjInVAO(const char* path)
{
	// temporary storage for positions, tex coords and normals
	vector<glm::vec3> pos_t;
	vector<glm::vec2> tex_t;
	vector<glm::vec3> norm_t;

	// reserve space to speed things up
	pos_t.reserve(10000);
	tex_t.reserve(10000);
	norm_t.reserve(10000);

	// map for storing temporary indices
	map<string, GLuint> ind_map;

	// indexed storage
	vector<VertexObject> buffer;
	vector<GLuint> indices;

	ifstream file(path);
	if (!file.is_open())
	{
		cout << "Could not open file: " << path << endl;
		Model null = { 0, { 0, 0 }, 0 };
		return null;
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

			int idx[2][3] = { { 0, 1, 2 }, {0, 2, 3} };

			//cout << "Faces: " << set[0] << ',' << set[1] << ',' << set[2] <<  endl;
			for (int j = 0; j < 2; ++j)
			{
				if (j == 1 && set[3].size() == 0)
					break;

				for (int i = 0; i < 3; ++i)
				{
					int id = idx[j][i];

					if (ind_map.count(set[id]))
					{
						indices.push_back(ind_map[set[id]]);
						//cout << "Face: " << set[i] << " found in map" << endl;
					}
					else
					{
						int i1 = 0, i2 = 0, i3 = 0;
						int matches = sscanf_s(set[id].c_str(), "%d/%d/%d", &i1, &i2, &i3);
						if (matches < 3)
							matches = sscanf_s(set[id].c_str(), "%d//%d", &i1, &i3);

						VertexObject vert;

						if (i1 > 0)
						{
							vert.pos = pos_t[i1 - 1];
							vert.tex = i2 != 0 ? tex_t[i2 - 1] : glm::vec2(0);
							vert.norm = i3 != 0 ? norm_t[i3 - 1] : glm::vec3(0);
						}
						else
						{
							vert.pos = pos_t[pos_t.size() + i1];
							vert.tex = i2 != 0 ? tex_t[tex_t.size() + i2] : glm::vec2(0);
							vert.norm = i3 != 0 ? norm_t[norm_t.size() + i3] : glm::vec3(0);
						}

						buffer.push_back(vert);

						GLuint index = buffer.size() - 1;
						ind_map[set[id]] = index;

						indices.push_back(index);
						//cout << "Face: " << set[i] << " added at index " << index << endl;
					}
				}
			}

		}
		else if (token == "g")
		{
			string name;
			lineStream >> name;

			cout << "Group started: " << name << endl;
		}
	}
	file.close();

	Model m;

	glGenVertexArrays(1, &m.vao);
	glBindVertexArray(m.vao);

	glGenBuffers(2, m.vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexObject)* buffer.size(), buffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* indices.size(), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexObject), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexObject), (GLvoid*)(sizeof(float)* 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexObject), (GLvoid*)(sizeof(float)* 5));

	glBindVertexArray(NULL);

	m.count = indices.size();

	return m;
}