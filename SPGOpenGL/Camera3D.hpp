#pragma once
#include "Camera.hpp"

class Camera3D : Camera {
private:
	glm::vec3 position, rotation, observedPoint, up;
	float fov;

public:
	Camera3D();
	Camera3D(glm::vec3 _position = glm::vec3(), glm::vec3 _rotation = glm::vec3());

	void setPosition(glm::fvec3 _position) { this->position = _position; }
	void setRotation(glm::fvec3 _rotation) { this->rotation = _rotation; }
	void setFov(float _fov) { this->fov = _fov; }
	void setUp(glm::vec3 _up) { this->up = _up; }
	void setObservedPoint(glm::vec3 _observedPoint) { this->observedPoint = _observedPoint; }

	glm::vec3 getPosition() { return this->position; }
	glm::vec3 getRotation() { return this->rotation; }
	float getFov() { return this->fov; }
	glm::vec3 getUp(){ return this->up; }
	glm::vec3 getObservedPoint() { return this->observedPoint; }

	void lookAt(glm::vec3 target);
};