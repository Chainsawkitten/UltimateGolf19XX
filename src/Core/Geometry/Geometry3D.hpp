#pragma once

#include "../Physics/AABB.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

namespace Geometry {
    /// Interface for renderable 3D geometry.
    /**
     * Can be rendered using the default shaders.
     */
    class Geometry3D {
        public:
            /// A vertex point.
            struct Vertex {
                /// Position.
                glm::vec3 position;
                /// %Texture coordinate.
                glm::vec2 textureCoordinate;
                /// Normal.
                glm::vec3 normal;
                /// Tangent vector.
                glm::vec3 tangent;
            };
            
            /// Destructor
            virtual ~Geometry3D();
    
            /// Get all the vertices.
            /**
             * @return Array of vertices
             */
            virtual Vertex* Vertices() const = 0;
    
            /// Get local space AABB for object
            /**
             * Create a local space AABB for geometry object
             */
            void CreateAabb();
    
            /// Get the number of vertices.
            /**
             * @return The number of vertices
             */
            virtual unsigned int VertexCount() const = 0;
    
            /// Get all the vertex indices.
            /**
             * @return Array of vertex indices
             */
            virtual unsigned int* Indices() const = 0;
    
            /// Get the number of indicies.
            /**
             * @return The number of vertex indices.
             */
            virtual unsigned int IndexCount() const = 0;
    
            /// Localspace AABB for the geometry
            Physics::AABB aabb;
    
            /// Get the vertex array.
            /**
             * @return The vertex array
             */
            GLuint VertexArray() const;
    
        protected:
            /// Generate vertex and index buffers.
            void GenerateBuffers();
    
            /// Generate vertex array.
            void GenerateVertexArray();
    
        private:
            GLuint vertexBuffer;
            GLuint indexBuffer;
            GLuint vertexArray;
    };
}

/** @} */
