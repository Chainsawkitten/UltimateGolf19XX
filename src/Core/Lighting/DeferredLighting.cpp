#include "DeferredLighting.hpp"
#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "Deferred.frag.hpp"

DeferredLighting::DeferredLighting(const glm::vec2& size) {
    vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(DEFERRED_FRAG, DEFERRED_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    
    rectangle = Resources().CreateRectangle();
    
    this->size = size;
    
    // Create the FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // Generate textures
    glGenTextures(NUM_TEXTURES, textures);
    glGenTextures(1, &depthHandle);
    
    unsigned int width = static_cast<unsigned int>(size.x);
    unsigned int height = static_cast<unsigned int>(size.y);
    AttachTexture(textures[DIFFUSE], width, height, GL_COLOR_ATTACHMENT0 + DIFFUSE, GL_RGB);
    AttachTexture(textures[NORMAL], width, height, GL_COLOR_ATTACHMENT0 + NORMAL, GL_RGB16F);
    AttachTexture(textures[SPECULAR], width, height, GL_COLOR_ATTACHMENT0 + SPECULAR, GL_RGB);
    
    // Bind depthHandle
    glBindTexture(GL_TEXTURE_2D, depthHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthHandle, 0);
    
    // Create and intialize draw buffers (output from geometry pass)
    GLenum drawBuffers[NUM_TEXTURES];
    for (unsigned int i = 0; i < NUM_TEXTURES; i++)
        drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    glDrawBuffers(NUM_TEXTURES, drawBuffers);
    
    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Frame buffer creation failed\n";
    
    // Default framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

DeferredLighting::~DeferredLighting() {
    if (fbo != 0)
        glDeleteFramebuffers(1, &fbo);
    
    if (textures[0] != 0)
        glDeleteTextures(NUM_TEXTURES, textures);
    
    if (depthHandle != 0)
        glDeleteTextures(1, &depthHandle);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    Resources().FreeRectangle();
}

GLuint DeferredLighting::Texture(TEXTURE_TYPE textureType) const {
    return textures[textureType];
}

void DeferredLighting::BindForWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void DeferredLighting::BindForReading() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    
    for (unsigned int i = 0; i < NUM_TEXTURES; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    
    glActiveTexture(GL_TEXTURE0 + NUM_TEXTURES);
    glBindTexture(GL_TEXTURE_2D, depthHandle);
}

void DeferredLighting::BindForTexReading() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void DeferredLighting::SetReadBuffer(TEXTURE_TYPE textureType){
    glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}

void DeferredLighting::ShowTextures(const glm::vec2& size) {
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    int halfWidth = width / 2;
    int halfHeight = height / 2;
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    BindForTexReading();
    
    SetReadBuffer(DeferredLighting::DIFFUSE);
    glBlitFramebuffer(0, 0, width, height, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    SetReadBuffer(DeferredLighting::NORMAL);
    glBlitFramebuffer(0, 0, width, height, 0, halfHeight, halfWidth, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    SetReadBuffer(DeferredLighting::SPECULAR);
    glBlitFramebuffer(0, 0, width, height, halfWidth, halfHeight, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
}

void DeferredLighting::ResetWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void DeferredLighting::Render(Camera* camera, const glm::vec2& screenSize) {
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    
    GLint oldDepthFunctionMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunctionMode);
    glDepthFunc(GL_ALWAYS);
    
    shaderProgram->Use();
    
    // Blending enabled for handling multiple light sources
    GLboolean blend = glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    BindLighting(camera, screenSize);
    BindForReading();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindVertexArray(rectangle->VertexArray());
    
    glDrawElements(GL_TRIANGLES, rectangle->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    if (!depthTest)
        glDisable(GL_DEPTH_TEST);
    if (!blend)
        glDisable(GL_BLEND);
    
    glDepthFunc(oldDepthFunctionMode);
}

void DeferredLighting::AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
}

void DeferredLighting::BindLighting(Camera* camera, const glm::vec2& screenSize){
    // Bind light information for lighting pass
    glm::mat4 view = camera->View();
    
    glUniform1i(shaderProgram->UniformLocation("tDiffuse"), DeferredLighting::DIFFUSE);
    glUniform1i(shaderProgram->UniformLocation("tNormals"), DeferredLighting::NORMAL);
    glUniform1i(shaderProgram->UniformLocation("tSpecular"), DeferredLighting::SPECULAR);
    glUniform1i(shaderProgram->UniformLocation("tDepth"), DeferredLighting::NUM_TEXTURES);
    
    glUniform4fv(shaderProgram->UniformLocation("lightPosition"), 1, &(view * light.position)[0]);
    glUniform3fv(shaderProgram->UniformLocation("lightIntensity"), 1, &light.intensity[0]);
    glUniform3fv(shaderProgram->UniformLocation("diffuseCoefficient"), 1, &light.diffuseCoefficient[0]);
    
    glUniformMatrix4fv(shaderProgram->UniformLocation("inverseProjectionMatrix"), 1, GL_FALSE, &glm::inverse(camera->Projection(screenSize))[0][0]);
}
