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
		
		Action action = Action(1);
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

		this->mesh.setupBuffers();
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

		this->mesh.setupBuffers();
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

		this->mesh.setupBuffers();
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

		this->mesh.setupBuffers();
	}

	void animate() {

		if (animation.currentFrame >= animation.actions[animation.currentAction].frameCount) {
			animation.currentFrame = 0;
			animation.currentAction++;
			//daca animatia ramane fara actiuni in lista, o ia de la capat
			if (animation.currentAction >= animation.actions.size()) {
				animation.currentAction = 0;

				generateNewGoal();
			}
		}

		position += animation.actions[animation.currentAction].move;
		//rotation += animation.actions[animation.currentAction].rotate;
		scale += animation.actions[animation.currentAction].rescale;

		// Calculate the direction towards the goal
		glm::vec3 goalDirection = glm::normalize(animation.actions[animation.currentAction].move);
		// Calculate the rotation to look at the goal
		glm::vec3 lookAtRotation = glm::vec3(0, 
			atan2(-goalDirection.z, goalDirection.x), 
			atan2(goalDirection.y, glm::length(glm::vec2(goalDirection.x, goalDirection.z))));
		// Set the rotation directly to the mesh to look at the goal
		mesh.rotation = lookAtRotation;

		animation.currentFrame++;

		mesh.position = position;
		//mesh.rotation = rotation;
		mesh.scale = scale;
	}

	void generateNewGoal() {
		animation.actions.clear();

		// Calculate a point within the specified boundary
		glm::vec3 goal;
		goal.x = (rand() % 200 - 100) / 10.0f;
		goal.y = (rand() % 600 - 300) / 100.0f;
		goal.z = (rand() % 200 - 100) / 100.0f;

		// Calculate the direction vector
		glm::vec3 direction = goal - position;
		direction = glm::normalize(direction);

		// Calculate the distance to the goal
		float distance = glm::distance(position, goal);

		// Calculate the number of frames needed to reach the goal
		int frameCount = static_cast<int>(distance * 100);

		// Calculate the speed needed to reach the goal in the given number of frames
		glm::vec3 move = direction * (distance / frameCount);

		// Create the action without rotation
		Action action = Action(move, glm::vec3(0), glm::vec3(0), frameCount);

		// Add the action to the animation
		animation.actions.push_back(action);
	}

	void render(glm::mat4 projectionViewMatrix, GLuint shader_programme) {
		mesh.render(projectionViewMatrix, shader_programme);
	}
};