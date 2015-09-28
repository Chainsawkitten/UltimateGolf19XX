#include "GolfBall.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Default3D.frag.hpp"

GolfBall::GolfBall() : GeometryObject(Resources().CreateCube()) {
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(DEFAULT3D_FRAG, DEFAULT3D_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    texture = Resources().CreateTexture2DFromFile("Resources/CGTextures/cliff.png");
    active = false;
}

GolfBall::~GolfBall() {
    Resources().FreeCube();
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
    
    Resources().FreeTexture2DFromFile(texture);
}

void GolfBall::Update(double time) {
    if (active) {
        Move(static_cast<float>(time) * velocity);
        
        /// @todo Calculate forces.
        
        glm::vec3 acceleration = glm::vec3(0.f, -9.82f, 0.f);
        velocity += acceleration * static_cast<float>(time);
    }
}

void GolfBall::Render(Camera *camera, const glm::vec2 &screenSize) const {
    shaderProgram->Use();
    glBindVertexArray(Geometry()->VertexArray());
    
    // Texture unit 0 is for base images.
    glUniform1i(shaderProgram->UniformLocation("baseImage"), 0);
    
    // Base image texture
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture->TextureID());
    
    // Model matrix, unique for each model.
    glm::mat4 model = ModelMatrix();
    
    // Send the matrices to the shader.
    glm::mat4 view = camera->View();
    glm::mat4 MV = view * model;
    glm::mat4 N = glm::transpose(glm::inverse(MV));
    
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelViewMatrix"), 1, GL_FALSE, &MV[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(N)[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);
    
    // Draw the triangles
    glDrawElements(GL_TRIANGLES, Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}

void GolfBall::Strike() {
    active = true;
    
    /// @todo Calculate velocity based on club mass, loft and velocity.
    
    velocity = glm::vec3(20.f, 5.f, 0.f);
}
