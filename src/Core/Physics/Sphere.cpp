#include "Sphere.hpp"
using namespace Physics;
Sphere::Sphere(glm::vec3 origin, float radius){
	this->position = origin;
	this->radius = radius;
}
Sphere::Sphere(){
	this->position = glm::vec3(0.f, 0.f, 0.f);
	this->radius = 0.f;
}