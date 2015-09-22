#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Object/Object.hpp"
#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

/// A camera through which the world can be rendered.
class Camera : public Object {
	public:
		/// Create new camera.
		Camera();

		/// Destructor
		virtual ~Camera() { }

		/// Get the direction in which the camera is currently facing.
		/**
		 * @return The direction in which the camera is currently facing
		 */
		glm::vec3 Forward() const;

		/// Get the direction to the right of the camera.
		/**
		 * @return The direction to the right of the camera
		 */
		glm::vec3 Right() const;

		/// Get the camera's up-vector.
		/**
		 * @return The camera's up-vector
		 */
		glm::vec3 Up() const;

		/// Get the camera's view matrix (translation and orientation).
		/**
		 * @return The view matrix
		 */
		glm::mat4 View() const;

		/// Get field of view.
		/**
		 * Default: 45.0
		 * @return Field of view (in degrees, 0.0-180.0)
		 */
		float FieldOfView() const;

		/// Set field of view.
		/**
		 * @param fieldOfView Field of view (in degrees, 0.0-180.0)
		 */
		void SetFieldOfView(float fieldOfView);

		/// Get near plane.
		/**
		 * Default: 0.1
		 * @return Near plane
		 */
		float NearPlane() const;

		/// Get far plane.
		/**
		 * Default: 100.0
		 * @return Far plane
		 */
		float FarPlane() const;

		/// Set near and far planes.
		/**
		 * @param near Near plane.
		 * @param far Far plane.
		 */
		void SetNearAndFarPlanes(float near, float far);

		/// Get the projection matrix.
		/**
		 * @param width Width of context to render to.
		 * @param height Height of context to render to.
		 * @return Projection matrix
		 */
		glm::mat4 Projection(int width, int height) const;

	private:
		float fieldOfView;
		float zNear;
		float zFar;
};

/** @} */

#endif
