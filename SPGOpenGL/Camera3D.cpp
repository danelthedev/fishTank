#include "Camera3D.hpp"

Camera3D::Camera3D() {
	this->position = glm::vec3();
	this->rotation = glm::vec3();
	this->up = glm::vec3(0, 1, 0);
	this->fov = glm::pi<float>() / 4;
	this->observedPoint = glm::vec3(0,0,-2);
}

Camera3D::Camera3D(glm::vec3 _position, glm::vec3 _rotation) {
	this->position = _position;
	this->rotation = _rotation;
	this->up = glm::vec3(0, 1, 0);
	this->fov = glm::pi<float>() / 4;
	this->observedPoint = glm::vec3(0, 0, -2);
}

void Camera3D::lookAt(glm::vec3 target) {
		this->rotation = glm::vec3(atan2(target.z - this->position.z, target.x - this->position.x), 
									atan2(target.y - this->position.y, target.x - this->position.x), 0);
}