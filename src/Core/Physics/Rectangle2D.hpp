#pragma once

#include "../Geometry/Geometry3D.hpp"
#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

namespace Physics {
    class Frustum;
    class Rectangle2D;
    
    /// 2D rectangle.
    /**
     * Used for view frustum culling.
     */
    class Rectangle2D {
        public:
            /// Origin.
            glm::vec2 origin;
    
            /// Dimensions.
            glm::vec2 dimensions;
    
            /// Create new rectangle.
            /**
             * @param origin Origin.
             * @param dimensions Dimensions.
             */
            Rectangle2D(const glm::vec2& origin = { 0.f, 0.f }, const glm::vec2& dimensions = { 0.f, 0.f });
            
            /// Create rectangle from geometry.
            /**
             * @param geometry Geometry to encapsulate.
             * @param matrix Transformation matrix to transform geometry with.
             */
            Rectangle2D(const Geometry::Geometry3D& geometry, const glm::mat4& matrix);
    
            /// Check collision between rectangle and a Frustum.
            /**
            * @param frustum The Frustum to check collision against.
            * @param height Height.
            * @return Whether there was a collision
            */
            bool Collide(const Frustum& frustum, const float height) const;
    
            /// Check collision between rectangle and another rectangle.
            /**
            * @param other The rectangle to check overlap against
            * @return Whether there was an overlap
            */
            bool Overlaps(const Rectangle2D& other);
    };
}

/** @} */
