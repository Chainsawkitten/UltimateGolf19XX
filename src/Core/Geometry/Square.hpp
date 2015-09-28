#pragma once

#include "Geometry3D.hpp"

/** @ingroup Core
 * @{
 */

namespace Geometry {
    /// A square.
    class Square : public Geometry3D {
        public:
            /// Create new square at origo.
            Square();
    
            /// Destructor
            ~Square();
    
            /// Get all the vertices.
            /**
            * @return Array of vertices
            */
            Vertex* Vertices() const;
    
            /// Get the number of vertices.
            /**
            * @return The number of vertices
            */
            unsigned int VertexCount() const;
    
            /// Get all the vertex indices.
            /**
            * @return Array of vertex indices
            */
            unsigned int* Indices() const;
    
            /// Get the number of indicies.
            /**
            * @return The number of vertex indices.
            */
            unsigned int IndexCount() const;
    
        private:
            Vertex *vertexData = nullptr;
            unsigned int vertexNr = 0;
    
            unsigned int* indexData = nullptr;
            unsigned int indexNr = 0;
    };
}

/** @} */
