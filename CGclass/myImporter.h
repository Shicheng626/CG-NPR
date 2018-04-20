#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>



// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

class myImporter{
public:

	vector<Vertex> vertices;
	vector<GLuint> indices;

void load_obj(string obj_path ) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coods;
	ifstream ifs;
	try {
		ifs.open(obj_path);
		string one_line;
		while (getline(ifs, one_line)) {
			stringstream ss(one_line);
			string type;
			ss >> type;
			if (type == "v") {
				glm::vec3 vert_pos;
				ss >> vert_pos[0] >> vert_pos[1] >> vert_pos[2];
				positions.push_back(vert_pos);
			}
			else if (type == "vt") {
				glm::vec2 tex_coord;
				ss >> tex_coord[0] >> tex_coord[1];
				tex_coods.push_back(tex_coord);
			}
			else if (type == "vn") {
				glm::vec3 vert_norm;
				ss >> vert_norm[0] >> vert_norm[1] >> vert_norm[2];
				normals.push_back(vert_norm);
			}
			else if (type == "f") {
				string s_vertex_0, s_vertex_1, s_vertex_2;
				ss >> s_vertex_0 >> s_vertex_1 >> s_vertex_2;
				int pos_idx, tex_idx, norm_idx;
				sscanf(s_vertex_0.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				// We have to use index -1 because the obj index starts at 1
				Vertex vertex_0;
				vertex_0.Position = positions[pos_idx - 1];
				vertex_0.TexCoords = tex_coods[tex_idx - 1];
				vertex_0.Normal = normals[norm_idx - 1];
				sscanf(s_vertex_1.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				Vertex vertex_1;
				vertex_1.Position = positions[pos_idx - 1];
				vertex_1.TexCoords = tex_coods[tex_idx - 1];
				vertex_1.Normal = normals[norm_idx - 1];
				sscanf(s_vertex_2.c_str(), "%d/%d/%d", &pos_idx, &tex_idx, &norm_idx);
				Vertex vertex_2;
				vertex_2.Position = positions[pos_idx - 1];
				vertex_2.TexCoords = tex_coods[tex_idx - 1];
				vertex_2.Normal = normals[norm_idx - 1];

				GetTangent(&vertex_0, &vertex_1, &vertex_2);

				vertices.push_back(vertex_0);
				indices.push_back(vertices.size() - 1);
				vertices.push_back(vertex_1);
				indices.push_back(vertices.size() - 1);
				vertices.push_back(vertex_2);
				indices.push_back(vertices.size() - 1);
			}
		}
	}
	catch (const std::exception&) {
		cout << "Error: Obj file cannot be read\n";
	}
}

void GetTangent(Vertex *v1,Vertex *v2, Vertex* v3)
{
	// positions
	glm::vec3 pos1 = v1->Position;
	glm::vec3 pos2 = v2->Position;
	glm::vec3 pos3 = v3->Position;
	// texture coordinates
	glm::vec2 uv1 = v1->TexCoords;
	glm::vec2 uv2 = v2->TexCoords;
	glm::vec2 uv3 = v3->TexCoords;

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// - triangle 1
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);
	v1->Tangent = tangent1;
	v2->Tangent = tangent1;
	v3->Tangent = tangent1;

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);
	v1->Bitangent = bitangent1;
	v2->Bitangent = bitangent1;
	v3->Bitangent = bitangent1;
}
};
