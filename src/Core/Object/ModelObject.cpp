#include "ModelObject.hpp"

ModelObject::ModelObject(const Geometry::Model* Model, std::string diffusePath, std::string normalPath, std::string specularPath):GeometryObject(Model){
	diffuse = Resources().CreateTexture2DFromFile(diffusePath);
	normal = Resources().CreateTexture2DFromFile(normalPath);
	specular = Resources().CreateTexture2DFromFile(specularPath);

	vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
	geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
	fragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
	shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });

	geometry = Geometry();
}

ModelObject::~ModelObject(){
	Resources().FreeTexture2DFromFile(diffuse);
	Resources().FreeTexture2DFromFile(normal);
	Resources().FreeTexture2DFromFile(specular);

	Resources().FreeShader(vertexShader);
	Resources().FreeShader(geometryShader);
	Resources().FreeShader(fragmentShader);
	Resources().FreeShaderProgram(shaderProgram);
}

void ModelObject::Render(Camera* camera, const glm::vec2& screenSize) const {
	shaderProgram->Use();
	
	//Set texture locations
	glUniform1i(shaderProgram->UniformLocation("diffuse"), 0);
	glUniform1i(shaderProgram->UniformLocation("normal"), 1);
	glUniform1i(shaderProgram->UniformLocation("specular"), 2);

	//Textures
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuse->TextureID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normal->TextureID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, specular->TextureID());

	glm::mat4 ModelView = camera->View()*ModelMatrix();
	glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));

	glUniformMatrix4fv(shaderProgram->UniformLocation("modelViewMatrix"), 1, GL_FALSE, &ModelView[0][0]);
	glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(Normal)[0][0]);
	glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);

	glBindVertexArray(geometry->VertexArray());

	glDrawElements(GL_TRIANGLES, geometry->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}