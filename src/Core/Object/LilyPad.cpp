#include "LilyPad.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"
#include "../Physics/Frustum.hpp"

LilyPad::LilyPad() {
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    geometry = Resources().CreateSquare();
    geometry->CreateAabb();
    texture = Resources().CreateTexture2DFromFile("Resources/CGTextures/LilyPads.png");
}

LilyPad::~LilyPad() {
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
    
    Resources().FreeSquare();
    Resources().FreeTexture2DFromFile(texture);
}

void LilyPad::Render(Camera* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) const {
    glm::mat4 projection = camera->Projection(screenSize);
    glm::mat4 view = camera->View();
    glm::mat4 model = ModelMatrix();
    
    // Frustum local to objects
    Physics::Frustum frustum(projection * view * model);
    
    if (frustum.Collide(geometry->aabb)) {
        shaderProgram->Use();
        
        // Set texture locations
        glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
        
        // Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->TextureID());
        
        glm::mat4 normal = glm::transpose(glm::inverse(view * ModelMatrix()));
        glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normal)[0][0]);
        glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &projection[0][0]);
        
        glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &clippingPlane[0]);
        
        glBindVertexArray(geometry->VertexArray());
        
        glDrawElements(GL_TRIANGLES, geometry->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}
