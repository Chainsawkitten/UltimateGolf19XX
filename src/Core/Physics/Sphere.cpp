#include "Sphere.hpp"
using namespace Physics;
Sphere::Sphere(glm::vec3 origin, float radius){
	this->position = origin;
	this->radius = radius;
}