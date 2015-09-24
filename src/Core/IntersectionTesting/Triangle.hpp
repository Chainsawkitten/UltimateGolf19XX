#pragma once

#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

/// A triangle.
/**
 * Used for intersection testing.
 */
class Triangle {
	public:
		/// First vertex.
		glm::vec3 v1;

		/// Second vertex.
		glm::vec3 v2;

		/// Third vertex.
		glm::vec3 v3;

		/// Create new triangle.
		/**
		 * @param v1 First vertex.
		 * @param v2 Second vertex.
		 * @param v3 Third vertex.
		 */
		Triangle(const glm::vec3& v1 = { 0.f, 0.f, 0.f }, const glm::vec3& v2 = { 0.f, 0.f, 0.f }, const glm::vec3& v3 = { 0.f, 0.f, 0.f });
};

/** @} */
