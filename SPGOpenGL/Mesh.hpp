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

	glm::vec3 position, rotation, scale;

	void load(std::string path) { 
		bool res = loadOBJ(path.c_str(), vertices, uvs, normals);
	}

	void setupBuffers()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(glm::vec3), &verticesData[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), NULL);

		// Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3));

		// Texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 6));
	}

	void render(glm::mat4 projectionViewMatrix, GLuint shader_programme) {

		GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
		GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");

		glm::mat4 modelMatrix = glm::mat4();
		modelMatrix *= glm::translate(position);
		modelMatrix *= glm::rotate(rotation.x, glm::vec3(1, 0, 0));
		modelMatrix *= glm::rotate(rotation.y, glm::vec3(0, 1, 0));
		modelMatrix *= glm::rotate(rotation.z, glm::vec3(0, 0, 1));
		modelMatrix *= glm::scale(scale);

		glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix * modelMatrix));
		glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glBindVertexArray(vao); // Bind the pre-created VAO
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

	Mesh() {
	}

	Mesh(std::string path) {
		load(path);
		updateVerticesData();

	}
	
	Mesh(glm::vec3 position, std::string path) {
		this->position = position;
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