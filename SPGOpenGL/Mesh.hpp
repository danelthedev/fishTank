#pragma once
#include <vector>
#include <string>
#include <glm/gtx/transform.hpp>
#include "objloader.hpp"
#include <GL/glew.h>

class Mesh {
public:
	GLuint vao, vbo;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> uvs;
	std::vector<glm::vec3> normals;


	glm::vec3 positionOffset;

	void load(std::string path) { bool res = loadOBJ(path.c_str(), vertices, uvs, normals); }
	void render() { glDrawArrays(GL_TRIANGLES, 0, vertices.size()); }

	Mesh() {

	}

	Mesh(std::string path) {
		load(path);
		updatePosition();
	}
	
	Mesh(glm::vec3 positionOffset, std::string path) {
		this->positionOffset = positionOffset;
		load(path);
		updatePosition();
	}

	void updatePosition() {
		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i] += positionOffset;
		}
	}
};