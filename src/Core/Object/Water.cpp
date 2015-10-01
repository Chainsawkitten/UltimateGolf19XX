#include "Water.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Default3D.geom.hpp"
#include "Water.frag.hpp"

Water::Water(const glm::vec2& screenSize) : GeometryObject(Resources().CreateSquare()) {
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(DEFAULT3D_GEOM, DEFAULT3D_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(WATER_FRAG, WATER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    SetRotation(0.f, 270.f, 0.f);
    
    refractionTarget = new RenderTarget(screenSize);
    reflectionTarget = new RenderTarget(screenSize * 0.25f);
}

Water::~Water() {
    delete refractionTarget;
    delete reflectionTarget;
    
    Resources().FreeSquare();
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
}

void Water::Render(Camera* camera, const glm::vec2& screenSize) const {
    refractionTarget->SetSource();
    reflectionTarget->SetSource();
    
    shaderProgram->Use();
    
    // Set texture locations
    glUniform1i(shaderProgram->UniformLocation("tRefraction"), 0);
    glUniform1i(shaderProgram->UniformLocation("tReflection"), 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, refractionTarget->ColorTexture());
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, reflectionTarget->ColorTexture());
    
    glUniform2fv(shaderProgram->UniformLocation("screenSize"), 1, &screenSize[0]);
    
    glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.f)[0]);
    
    // Send matrices to shader.
    glm::mat4 Normal = glm::transpose(glm::inverse(camera->View() * ModelMatrix()));
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &ModelMatrix()[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &camera->View()[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(Normal)[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);
    
    glBindVertexArray(Geometry()->VertexArray());
    
    glDrawElements(GL_TRIANGLES, Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
}

RenderTarget* Water::RefractionTarget() const {
    return refractionTarget;
}

RenderTarget* Water::ReflectionTarget() const {
    return reflectionTarget;
}

glm::vec4 Water::RefractionClippingPlane() const {
    /// @todo Don't hardcore clipping planes
    return glm::vec4(0.f, -1.f, 0.f, Position().y);
}

glm::vec4 Water::ReflectionClippingPlane() const {
    /// @todo Don't hardcore clipping planes
    return glm::vec4(0.f, 1.f, 0.f, -Position().y);
}
