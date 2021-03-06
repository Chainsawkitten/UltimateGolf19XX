#pragma once

#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

namespace Physics {
    class AABB;
    class Rectangle2D;
    
    /// A viewing frustum.
    /**
     * Used for frustum culling.
     */
    class Frustum {
        public:
            /// Create new frustum.
            /**
             * @param matrix View-projection matrix to create frustum planes from.
             */
            Frustum(const glm::mat4& matrix);
    
            /// Check collision between frustum and an AABB.
            /**
             * @param aabb The AABB to check collision against.
             * @return Whether there was a collision
             */
            bool Collide(const AABB& aabb) const;
    
            /// Check collision between frustum and a rectangle.
            /**
            * @param rectangle The rectangle to check collision against.
            * @param height Height.
            * @return Whether there was a collision
            */
            bool Collide(const Rectangle2D& rectangle, float height) const;
    
        private:
            glm::vec4 planes[6];
    
            // The distance returned is the signed distance in normal vector units.
            // To ensure the "true" distance the plane equation has to be normalized.
            static float DistanceToPoint(const glm::vec4& plane, const glm::vec3& point);
    };
}

/** @} */
