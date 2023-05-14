#pragma once
#include <glm/gtx/transform.hpp>

class Light {
private:
	glm::vec3 position;
public:
	Light();
	Light(glm::vec3 _position);
	void setPosition(glm::vec3 _position) { this->position = _position; }
	glm::vec3 getPosition() { return this->position; }	
};