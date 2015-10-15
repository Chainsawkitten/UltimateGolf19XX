#include "Duck.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"

Duck::Duck() {
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    geometry = Resources().CreateOBJModel("Resources/Models/Duck/Duck.obj");
    texture = Resources().CreateTexture2DFromFile("Resources/Models/Duck/Diffuse.png");
}

Duck::~Duck() {
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
    
    Resources().FreeOBJModel(geometry);
    Resources().FreeTexture2DFromFile(texture);
}

void Duck::Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const {
    shaderProgram->Use();
    
    //Set texture locations
    glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
    
    //Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->TextureID());
    
    glm::mat4 view = camera->View();
    glm::mat4 normal = glm::transpose(glm::inverse(view * ModelMatrix()));
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &ModelMatrix()[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normal)[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);
    
    glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &clippingPlane[0]);
    
    glBindVertexArray(geometry->VertexArray());
    
    glDrawElements(GL_TRIANGLES, geometry->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}
