#include "TerrainObject.hpp"
#include "../Geometry/Terrain.hpp"

TerrainObject::TerrainObject(const Terrain* terrain) : GeometryObject(terrain) {
	this->terrain = terrain;
	blendMap = Resources().CreateTexture2DFromFile("Resources/Terrain/blendmap.tga");
	grassTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/grass.tga");
	cliffTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/cliff.tga");
	sandTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/sand.tga");
	snowTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/snow.tga");
}

const Geometry::Geometry3D* TerrainObject::geometry() const {
	return terrain;
}

float TerrainObject::getY(float x, float z) const {
	float xInTerrain = (x - Position().x) / Scale().x + 0.5f;
	float zInTerrain = (z - Position().z) / Scale().z + 0.5f;

	return terrain->GetY(xInTerrain, zInTerrain) * Scale().y + Position().y;
}

void Render(ShaderProgram* shaderProgram, Camera* camera, const glm::vec2& screenSize) const{

}