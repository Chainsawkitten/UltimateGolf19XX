#pragma once

#include "GeometryObject.hpp"
#include "../Geometry/Terrain.hpp"

/** @ingroup Core
 * @{
 */

/// A terrain object in the game world.
class TerrainObject : public GeometryObject {
	public:
		/// Create new terrain object.
		/**
		 * @param terrain The Terrain the object should render.
		 */
		TerrainObject(const Geometry::Terrain* terrain);
        
		/// Destroys terrain object.
		~TerrainObject();
        
		/// Get renderable Geometry.
		/**
		 * @return The renderable Geometry.
		 */
		const Geometry::Geometry3D* Geometry() const;
        
		/// Gets the height of the terrain at a given location.
		/**
		 * If the position is outside the terrain, 0.0 is returned.
		 * @param x X-position in the world.
		 * @param z Z-position in the world.
		 * @return Height of the terrain.
		 */
		float GetY(float x, float z) const;
        
		/// Renders terrain.
		void Render(Camera* camera, const glm::vec2& screenSize) const;
        
	private:
		Texture2D* blendMap;
		Texture2D* grassTexture;
		Texture2D* cliffTexture;
		Texture2D* sandTexture;
		Texture2D* snowTexture;
        
		Shader* vertexShader;
		Shader* fragmentShader;
		Shader* geometryShader;
		ShaderProgram* shaderProgram;
        
		const Geometry::Terrain* terrain;
};

/** @} */
