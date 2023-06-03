#pragma once
#include <vector>
#include <string>
#include <glm/gtx/transform.hpp>
#include "objloader.hpp"
#include <GL/glew.h>
#include "Mesh.hpp"
#include "Texture.hpp"

struct Action {
	glm::vec3 move, rotate, rescale;
	int frameCount;

	Action(glm::vec3 move, glm::vec3 rotate, glm::vec3 rescale, int frameCount) {
		this->move = move;
		this->rotate = rotate;
		this->rescale = rescale;

		this->frameCount = frameCount;
	}
	
	Action(int frameCount) {
		this->frameCount = frameCount;
		move = glm::vec3(0, 0, 0);
		rotate = glm::vec3(0, 0, 0);
		rescale = glm::vec3(0, 0, 0);
	}

	Action() {
		this->frameCount = 0;
		move = glm::vec3(0, 0, 0);
		rotate = glm::vec3(0, 0, 0);
		rescale = glm::vec3(0, 0, 0);
	}
};

struct Animation {
	std::vector<Action> actions;
	int currentAction, currentFrame;

	Animation() {
		currentAction = 0;
		currentFrame = 0;
	}
};

class Fish {
public:
	GLuint shader_programme;

	Mesh mesh;
	Texture texture;
	glm::vec3 position, rotation, scale;
	Animation animation;

	void configureBasicAnimation() {
		animation = Animation();
		
		Action action = Action(500);
		action.rescale = glm::vec3(0.004, 0.004, 0.004);
		action.rotate = glm::vec3(0, -0.01, 0);

		animation.actions.push_back(action);

		action = Action(500);
		action.rescale = glm::vec3(-0.004, -0.004, -0.004);
		action.rotate = glm::vec3(0, -0.01, 0);
		animation.actions.push_back(action);
	}

	Fish() {
		configureBasicAnimation();
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
		this->mesh.position = position;
		this->mesh.rotation = rotation;
		this->mesh.scale = scale;
	}

	Fish(Mesh mesh) {
		configureBasicAnimation();
		this->mesh = mesh;

		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
		this->mesh.position = position;
		this->mesh.rotation = rotation;
		this->mesh.scale = scale;
	}

	Fish(Mesh mesh, glm::vec3 position) {
		configureBasicAnimation();
		this->mesh = mesh;

		this->position = position;
		this->mesh.position = position;
		
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
		this->mesh.rotation = rotation;
		this->mesh.scale = scale;
	}

	Fish(Mesh mesh, std::string texturePath, glm::vec3 position, GLuint shader_programme) {
		configureBasicAnimation();
		this->mesh = mesh;

		this->position = position;
		this->mesh.position = position;

		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
		this->mesh.rotation = rotation;
		this->mesh.scale = scale;

		this->texture = Texture();
		this->texture.load(texturePath.c_str(), shader_programme);
	}

	Fish(Mesh mesh, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
		configureBasicAnimation();
		this->mesh = mesh;

		this->mesh.position = position;
		this->position = position;

		this->mesh.scale = scale;
		this->scale = scale;

		this->mesh.rotation = rotation;
		this->rotation = rotation;
	}


	void animate() {

		if (animation.currentFrame >= animation.actions[animation.currentAction].frameCount) {
			animation.currentFrame = 0;
			animation.currentAction++;
			if (animation.currentAction >= animation.actions.size()) {
				animation.currentAction = 0;
			}
		}

		position += animation.actions[animation.currentAction].move;
		rotation += animation.actions[animation.currentAction].rotate;
		scale += animation.actions[animation.currentAction].rescale;


		animation.currentFrame++;

		mesh.position = position;
		mesh.rotation = rotation;
		mesh.scale = scale;
	}

	void render(glm::mat4 projectionViewMatrix, GLuint shader_programme) {
		mesh.render(projectionViewMatrix, shader_programme);
	}
};