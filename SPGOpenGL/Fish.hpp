#pragma once
#include <vector>
#include <string>
#include <glm/gtx/transform.hpp>
#include "objloader.hpp"
#include <GL/glew.h>
#include "Mesh.hpp"

struct Direction {
	glm::vec3 direction;
	int frameCount;

	Direction(glm::vec3 direction, int frameCount) {
		this->direction = direction;
		this->frameCount = frameCount;
	}
};

struct Animation {
	std::vector<Direction> directions;
	int currentDirection, currentFrame;

	Animation() {
		currentDirection = 0;
		currentFrame = 0;
	}
};

class Fish {
public:
	Mesh mesh;
	glm::vec3 positionOffset;
	Animation animation;

	Fish() {
		animation = Animation();
		animation.directions.push_back(Direction(glm::vec3(0.0005, 0, 0), 1000));
		animation.directions.push_back(Direction(glm::vec3(-0.0005, 0, 0), 1000));
	}

	Fish(Mesh mesh) {
		animation.directions.push_back(Direction(glm::vec3(0.0005, 0, 0), 1000));
		animation.directions.push_back(Direction(glm::vec3(-0.0005, 0, 0), 1000));

		this->mesh = mesh;
	}

	Fish(Mesh mesh, glm::vec3 positionOffset) {
		animation.directions.push_back(Direction(glm::vec3(0.01, 0, 0), 1000));
		animation.directions.push_back(Direction(glm::vec3(-0.01, 0, 0), 1000));

		this->mesh = mesh;
		this->mesh.positionOffset = positionOffset;
		this->positionOffset = positionOffset;
		this->mesh.updatePosition();
	}

	void animate() {

		if (animation.currentFrame >= animation.directions[animation.currentDirection].frameCount) {
			animation.currentFrame = 0;
			animation.currentDirection++;
			if (animation.currentDirection >= animation.directions.size()) {
				animation.currentDirection = 0;
			}
		}

		positionOffset += animation.directions[animation.currentDirection].direction;


		animation.currentFrame++;

		mesh.positionOffset = positionOffset;
		//mesh.updatePosition();
	}

};