#include "Light.hpp"

Light::Light()
{
	this->position = glm::vec3();
}

Light::Light(glm::vec3 _position)
{
	this->position = _position;
}