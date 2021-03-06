#include "ModelObject.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "NormalSpecularMap.frag.hpp"

ModelObject::ModelObject(const Geometry::Model* model, std::string diffusePath, std::string normalPath, std::string specularPath) : GeometryObject(model) {
    diffuse = Resources().CreateTexture2DFromFile(diffusePath);
    normal = Resources().CreateTexture2DFromFile(normalPath);
    specular = Resources().CreateTexture2DFromFile(specularPath);
    
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(NORMALSPECULARMAP_FRAG, NORMALSPECULARMAP_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
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

void ModelObject::Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const {
    shaderProgram->Use();
    
    //Set texture locations
    glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
    glUniform1i(shaderProgram->UniformLocation("normalMap"), 1);
    glUniform1i(shaderProgram->UniformLocation("specularMap"), 2);
    
    //Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse->TextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal->TextureID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specular->TextureID());
    
    glm::mat4 view = camera->View();
    glm::mat4 normal = glm::transpose(glm::inverse(view * ModelMatrix()));
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &ModelMatrix()[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normal)[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);
    
    glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &clippingPlane[0]);
    
    glBindVertexArray(Geometry()->VertexArray());
    
    glDrawElements(GL_TRIANGLES, Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}
