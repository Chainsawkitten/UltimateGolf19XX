#include "Water.hpp"
#include "../Resources.hpp"
#include "Default3D.vert.hpp"
#include "Water.geom.hpp"
#include "Water.frag.hpp"
#include <cmath>

Water::Water(const glm::vec2& screenSize) : GeometryObject(Resources().CreateSquare()) {
    vertexShader = Resources().CreateShader(DEFAULT3D_VERT, DEFAULT3D_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(WATER_GEOM, WATER_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(WATER_FRAG, WATER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    SetRotation(0.f, 270.f, 0.f);
    
    refractionTarget = new RenderTarget(screenSize);
    reflectionTarget = new RenderTarget(screenSize * 0.5f);
    
    waterTexture = Resources().CreateTexture2DFromFile("Resources/Terrain/Water.png");
    dudvMap = Resources().CreateTexture2DFromFile("Resources/Terrain/WaterDUDV.png");
    normalMap = Resources().CreateTexture2DFromFile("Resources/Terrain/WaterNormal.png");
    textureRepeat = glm::vec2(1.f, 1.f);
    
    texOffset = glm::vec2(0.f, 0.f);
    moveFactor = 0.f;
}

Water::~Water() {
    Resources().FreeTexture2DFromFile(waterTexture);
    Resources().FreeTexture2DFromFile(dudvMap);
    Resources().FreeTexture2DFromFile(normalMap);
    
    delete refractionTarget;
    delete reflectionTarget;
    
    Resources().FreeSquare();
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
}

void Water::Update(double time, const glm::vec3& wind) {
    texOffset += 0.02f * static_cast<float>(time) * -glm::vec2(wind.x, wind.z);
    texOffset.x = fmod(texOffset.x, 1.f);
    texOffset.y = fmod(texOffset.y, 1.f);
    
    moveFactor += static_cast<float>(time) * 0.02f;
    moveFactor = fmod(moveFactor, 1.f);
}

void Water::Render(Camera* camera, const Light& light, const glm::vec2& screenSize) const {
    refractionTarget->SetSource();
    reflectionTarget->SetSource();
    
    shaderProgram->Use();
    
    // Blending
    GLboolean blending;
    glGetBooleanv(GL_BLEND, &blending);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set texture locations
    glUniform1i(shaderProgram->UniformLocation("tRefraction"), 0);
    glUniform1i(shaderProgram->UniformLocation("tReflection"), 1);
    glUniform1i(shaderProgram->UniformLocation("tDuDvMap"), 2);
    glUniform1i(shaderProgram->UniformLocation("tWater"), 3);
    glUniform1i(shaderProgram->UniformLocation("tNormalMap"), 4);
    glUniform1i(shaderProgram->UniformLocation("tDepthMap"), 5);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, refractionTarget->ColorTexture());
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, reflectionTarget->ColorTexture());
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMap->TextureID());
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, waterTexture->TextureID());
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, normalMap->TextureID());
    
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, refractionTarget->DepthTexture());
    
    glUniform2fv(shaderProgram->UniformLocation("screenSize"), 1, &screenSize[0]);
    glUniform2fv(shaderProgram->UniformLocation("textureRepeat"), 1, &textureRepeat[0]);
    glUniform2fv(shaderProgram->UniformLocation("texOffset"), 1, &texOffset[0]);
    glUniform4fv(shaderProgram->UniformLocation("clippingPlane"), 1, &glm::vec4(0.f, 0.f, 0.f, 0.f)[0]);
    glUniform1f(shaderProgram->UniformLocation("moveFactor"), moveFactor);
    glUniform1f(shaderProgram->UniformLocation("zNear"), camera->NearPlane());
    glUniform1f(shaderProgram->UniformLocation("zFar"), camera->FarPlane());
    
    // Send matrices to shader.
    glm::mat4 view = camera->View();
    glm::mat4 normal = glm::transpose(glm::inverse(view * ModelMatrix()));
    glUniformMatrix4fv(shaderProgram->UniformLocation("modelMatrix"), 1, GL_FALSE, &ModelMatrix()[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix3fv(shaderProgram->UniformLocation("normalMatrix"), 1, GL_FALSE, &glm::mat3(normal)[0][0]);
    glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(screenSize)[0][0]);
    
    glUniform4fv(shaderProgram->UniformLocation("lightPosition"), 1, &(view * light.position)[0]);
    glUniform3fv(shaderProgram->UniformLocation("lightIntensity"), 1, &light.intensity[0]);
    
    glBindVertexArray(Geometry()->VertexArray());
    
    glDrawElements(GL_TRIANGLES, Geometry()->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    if (!blending)
        glDisable(GL_BLEND);
}

RenderTarget* Water::RefractionTarget() const {
    return refractionTarget;
}

RenderTarget* Water::ReflectionTarget() const {
    return reflectionTarget;
}

glm::vec4 Water::RefractionClippingPlane() const {
    /// @todo Don't hardcore clipping planes
    return glm::vec4(0.f, -1.f, 0.f, Position().y + 0.5f);
}

glm::vec4 Water::ReflectionClippingPlane() const {
    /// @todo Don't hardcore clipping planes
    return glm::vec4(0.f, 1.f, 0.f, -Position().y + 0.1f);
}

void Water::SetTextureRepeat(const glm::vec2& textureRepeat) {
    this->textureRepeat = textureRepeat;
}
