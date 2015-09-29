#pragma once
#include <glm\glm.hpp>

namespace Physics{
	class Sphere{
	public:
		/// Create new sphere.
		/**
		* @param origin The spheres origin.
		* @param radius The radius of the sphere.
		*/
		Sphere(glm::vec3 origin, float radius);

		glm::vec3 position;
		float radius;
	private:
	};
}

/** @} */
