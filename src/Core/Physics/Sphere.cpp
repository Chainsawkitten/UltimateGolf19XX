#include "Sphere.hpp"

namespace Physics {
    Sphere::Sphere(glm::vec3 origin, float radius) {
        this->position = origin;
        this->radius = radius;
    }

	bool Sphere::SphereVsTriangle(Triangle triangle){
		return true;
	}
}
