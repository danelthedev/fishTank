#pragma once
#include <vector>
#include <string>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objloader.hpp"
#include <GL/glew.h>

class Mesh {
public:
	GLuint vao, vbo;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> uvs;
	std::vector<glm::vec3> normals;

	std::vector<glm::vec3> verticesData;

	glm::vec3 positionOffset;
	float rotation, scale = 3;

	void load(std::string path) { 
		bool res = loadOBJ(path.c_str(), vertices, uvs, normals);
	}

	void render(glm::mat4 projectionViewMatrix, GLuint modelMatrixLoc) {

		glm::mat4 modelMatrix = glm::mat4();
		modelMatrix *= glm::translate(positionOffset);
		modelMatrix *= glm::scale(glm::vec3(scale, scale, scale));
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix * modelMatrix));
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(glm::vec3), &verticesData[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), NULL);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 6));

		glDrawArrays(GL_TRIANGLES, 0, vertices.size()); 
	}

	Mesh() {
	}

	Mesh(std::string path) {
		load(path);
		updateVerticesData();

	}
	
	Mesh(glm::vec3 positionOffset, std::string path) {
		this->positionOffset = positionOffset;
		load(path);
		updateVerticesData();

	}

	void updateVerticesData() {
		for (int i = 0; i < vertices.size(); i++)
		{
			verticesData.push_back(vertices[i]);
			verticesData.push_back(normals[i]);
			verticesData.push_back(uvs[i]);
		}
	}

};