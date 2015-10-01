#include "Water.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Water.frag.hpp"

Water::Water() : GeometryObject(Resources().CreateSquare()) {
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(WATER_FRAG, WATER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    SetRotation(0.f, 270.f, 0.f);
}

Water::~Water() {
    Resources().FreeSquare();
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
}

void Water::Render(Camera* camera, const glm::vec2& screenSize) const {
    shaderProgram->Use();
	
	//Set texture locations
	glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
	glUniform1i(shaderProgram->UniformLocation("normalMap"), 1);
	glUniform1i(shaderProgram->UniformLocation("specularMap"), 2);

	glm::mat4 ModelView = camera->View() * ModelMatrix();
	glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));

	glUniformMatrix4fv(shaderProgram->UniformLocation("modelViewMatrix"), 1, GL_FALSE, &ModelView[0][0]);
	glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(Normal)[0][0]);
	glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);

	glBindVertexArray(Geometry()->VertexArray());

	glDrawElements(GL_TRIANGLES, Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}
