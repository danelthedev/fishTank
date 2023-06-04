#pragma once
#include <vector>
#include <string>
#include <glm/gtx/transform.hpp>
#include "objloader.hpp"
#include <GL/glew.h>
#include "Mesh.hpp"
#include "Texture.hpp"

class Prop {
public:
	GLuint shader_programme;

	Mesh mesh;
	Texture texture;
	glm::vec3 position, rotation, scale;

	Prop(Mesh mesh, std::string texturePath, glm::vec3 position, GLuint shader_programme) {
		this->mesh = mesh;

		this->position = position;
		this->mesh.position = position;

		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
		this->mesh.rotation = rotation;
		this->mesh.scale = scale;

		this->texture = Texture();
		this->texture.load(texturePath.c_str(), shader_programme);

		this->mesh.setupBuffers();
	}

	void render(glm::mat4 projectionViewMatrix, GLuint shader_programme) {
		mesh.render(projectionViewMatrix, shader_programme);
	}
};