#include "TerrainObject.hpp"

TerrainObject::TerrainObject(const Terrain* terrain) : GeometryObject(terrain) {
	this->terrain = terrain;

	//Allocate resources from manager.
	blendMap = Resources().CreateTexture2DFromFile("Resources/Terrain/blendmap.tga");
	grassTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/grass.tga");
	cliffTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/cliff.tga");
	sandTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/sand.tga");
	snowTexture = Resources().CreateTexture2DFromFile("Resources/CGTextures/snow.tga");

	//Allocate shaders
	vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH,GL_VERTEX_SHADER);
	geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
	fragmentShader = Resources().CreateShader(BLENDMAP_FRAG, BLENDMAP_FRAG_LENGTH, GL_FRAGMENT_SHADER);
	shaderProgram = Resources().CreateShaderProgram({vertexShader, geometryShader, fragmentShader});

	SetPosition(0.f, -5.f, 0.f);
	SetScale(50.f, 10.f, 50.f);
}

const Geometry::Geometry3D* TerrainObject::geometry() const {
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

float TerrainObject::getY(float x, float z) const {
	float xInTerrain = (x - Position().x) / Scale().x + 0.5f;
	float zInTerrain = (z - Position().z) / Scale().z + 0.5f;

	return terrain->GetY(xInTerrain, zInTerrain) * Scale().y + Position().y;
}

void TerrainObject::Render(Camera* camera, const glm::vec2& screenSize) const{
	shaderProgram->Use();
	//Set texture locations
	glUniform1i(shaderProgram->UniformLocation("blendMapTexture"), 0);
	glUniform1i(shaderProgram->UniformLocation("redTexture"), 1);
	glUniform1i(shaderProgram->UniformLocation("greenTexture"), 2);
	glUniform1i(shaderProgram->UniformLocation("blueTexture"), 3);
	glUniform1i(shaderProgram->UniformLocation("alphaTexture"), 4);

	//Blend map
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

	//Texture Repeat
	glUniform2fv(shaderProgram->UniformLocation("textureRepeat"), 1, &terrain->TextureRepeat()[0]);

	//Send matrices to shader.
	glm::mat4 ModelView = camera->View()*ModelMatrix();
	glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));

	glUniformMatrix4fv(shaderProgram->UniformLocation("modelViewMatrix"), 1, GL_FALSE, &ModelView[0][0]);
	glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(Normal)[0][0]);
	glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);

	glBindVertexArray(geometry()->VertexArray());

	glDrawElements(GL_TRIANGLES, this->geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}