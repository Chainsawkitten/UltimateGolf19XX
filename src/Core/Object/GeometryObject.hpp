#pragma once

#include "Object.hpp"
#include "../Geometry/Geometry3D.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Camera/Camera.hpp"

/** @ingroup Core
 * @{
 */

/// A game object with renderable 3D geometry.
class GeometryObject : public Object {
	public:
		/// Create new geometry object.
		/**
		 * @param geometry The geometry the object should render.
		 */
		GeometryObject(const Geometry::Geometry3D* geometry);

		/// Get renderable Geometry.
		/**
		 * @return The renderable Geometry.
		 */
		virtual const Geometry::Geometry3D* Geometry() const;

		/// Render the object.
        /**
         * @param shaderProgram %Shader program to render the object with.
         * @param camera %Camera to render through.
         * @param screenSize Size of the screen in pixels.
         */
		virtual void Render(ShaderProgram* shaderProgram, Camera* camera, const glm::vec2& screenSize) const;

		/// Get relative drawing distance (compared to other objects).
		/**
		 * Default: 1.0
		 * @return Relative drawing distance
		 */
		float RelativeDrawingDistance() const;

		/// Set relative drawing distance (compared to other objects).
		/**
		 * @param drawingDistance Relative drawing distance.
		 */
		void SetRelativeDrawingDistance(float drawingDistance);

	private:
		const Geometry::Geometry3D* geometry;
		float drawingDistance;
};

/** @} */