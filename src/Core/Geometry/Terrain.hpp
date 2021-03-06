#pragma once

#include "../Geometry/Geometry3D.hpp"
#include "../Texture/Texture2D.hpp"

/** @ingroup Core
 * @{
 */

namespace Geometry {
    /// A terrain mesh.
    class Terrain : public Geometry3D {
        public:
            /// Create new terrain from the given height map.
            /**
             * @param filename Filename (relative or absolute) to height map texture.
             */
            Terrain(const char* filename);
    
            /// Destructor
            ~Terrain();
    
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
    
            /// Gets the height of the terrain at a given location.
            /**
             * If the position is outside the terrain, 0.0 is returned.
             * @param x X-position on the terrain.
             * @param z Z-position on the terrain.
             * @return Height of the terrain (0.0 - 1.0).
             */
            float GetY(float x, float z) const;
    
			/// Gets the normal at given location
			/**
			 * If the position is outside the terrain, 0.0, 0.0, 0.0 is returned.
			 * @param x X-position in the world.
			 * @param z Z-position in the world.
			 * @return Normal at that point.
			 */
			glm::vec3 GetNormal(float x, float z) const;

            /// Get how many times the texture should repeat.
            /**
             * Default: (1.0, 1.0)
             * @return How many times the texture should repeat
             */
            glm::vec2 TextureRepeat() const;
    
            /// Set how many times the texture should repeat.
            /**
             * @param repeat How many times the texture should repeat.
             */
            void SetTextureRepeat(const glm::vec2& repeat);
            
        private:
            void GenerateVertices();
            void GenerateIndices();
            void Filter3x3();
            float SampleHeight(int x, int y) const;
            void CalculateNormals();
    
            Vertex *vertexData;
            unsigned int vertexNr;
    
            unsigned int* indexData;
            unsigned int indexNr;
    
            float** heightMap;
            glm::vec3** normals;
            glm::vec3** tangents;
            int width;
            int height;
    
            glm::vec2 textureRepeat;
    };
}

/** @} */
