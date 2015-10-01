#include "TerrainObject.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Blendmap.frag.hpp"
#include <glm/glm.hpp>

TerrainObject::TerrainObject(const Geometry::Terrain* terrain) : GeometryObject(terrain) {
	this->terrain = terrain;

	// Allocate resources from manager.
	blendMap = Resources().CreateTexture2DFromFile("Resources/Terrain/blendmap.png");
	grassTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/grass.png");
	cliffTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/cliff.png");
	sandTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/sand.png");
	snowTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/snow.png");

	// Allocate shaders
	vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH,GL_VERTEX_SHADER);
	geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
	fragmentShader = Resources().CreateShader(BLENDMAP_FRAG, BLENDMAP_FRAG_LENGTH, GL_FRAGMENT_SHADER);
	shaderProgram = Resources().CreateShaderProgram({vertexShader, geometryShader, fragmentShader});
}

const Geometry::Geometry3D* TerrainObject::Geometry() const {
	return terrain;
}

TerrainObject::~TerrainObject(){
	Resources().FreeTexture2DFromFile(blendMap);
	Resources().FreeTexture2DFromFile(grassTexture);
	Resources().FreeTexture2DFromFile(cliffTexture);
	Resources().FreeTexture2DFromFile(sandTexture);
	Resources().FreeTexture2DFromFile(snowTexture);

	Resources().FreeShaderProgram(shaderProgram);
	Resources().FreeShader(vertexShader);
	Resources().FreeShader(geometryShader);
	Resources().FreeShader(fragmentShader);
}

float TerrainObject::GetY(float x, float z) const {
	float xInTerrain = (x - Position().x) / Scale().x + 0.5f;
	float zInTerrain = (z - Position().z) / Scale().z + 0.5f;

	return terrain->GetY(xInTerrain, zInTerrain) * Scale().y + Position().y;
}

void TerrainObject::Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const{
	shaderProgram->Use();
	// Set texture locations
	glUniform1i(shaderProgram->UniformLocation("blendMapTexture"), 0);
	glUniform1i(shaderProgram->UniformLocation("redTexture"), 1);
	glUniform1i(shaderProgram->UniformLocation("greenTexture"), 2);
	glUniform1i(shaderProgram->UniformLocation("blueTexture"), 3);
	glUniform1i(shaderProgram->UniformLocation("alphaTexture"), 4);

	// Blend map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blendMap->TextureID());

	// Textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grassTexture->TextureID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, cliffTexture->TextureID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, sandTexture->TextureID());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, snowTexture->TextureID());

	// Texture Repeat
	glUniform2fv(shaderProgram->UniformLocation("textureRepeat"), 1, &terrain->TextureRepeat()[0]);

	// Send matrices to shader.
	glm::mat4 Normal = glm::transpose(glm::inverse(camera->View()*  ModelMatrix()));
	glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &ModelMatrix()[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &camera->View()[0][0]);
	glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(Normal)[0][0]);
	glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);
    
    glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &clippingPlane[0]);

	glBindVertexArray(Geometry()->VertexArray());

	glDrawElements(GL_TRIANGLES, Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}
