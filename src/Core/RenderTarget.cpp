#include "RenderTarget.hpp"
#include "Util/Log.hpp"
#include "Resources.hpp"
#include "Post.vert.hpp"
#include "PostCopy.frag.hpp"

RenderTarget::RenderTarget(const glm::vec2 &size) {
    width = static_cast<int>(size.x);
    height = static_cast<int>(size.y);
    
    // Color buffer.
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Depth buffer.
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Framebuffer objects
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
    
    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Framebuffer creation failed\n";
    
    // Default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(POSTCOPY_FRAG, POSTCOPY_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    
    rectangle = Resources().CreateRectangle();
}

RenderTarget::~RenderTarget() {
    glDeleteTextures(1, &depthBuffer);
    glDeleteTextures(1, &colorBuffer);
    glDeleteFramebuffers(1, &frameBuffer);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    Resources().FreeRectangle();
}

void RenderTarget::SetTarget() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
}

void RenderTarget::SetSource() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
}

glm::vec2 RenderTarget::Size() const {
    return glm::vec2(static_cast<float>(width), static_cast<float>(height));
}

GLuint RenderTarget::ColorTexture() const {
    return colorBuffer;
}

GLuint RenderTarget::DepthTexture() const {
    return depthBuffer;
}

void RenderTarget::Render() {
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    
    GLint oldDepthFunctionMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunctionMode);
    glDepthFunc(GL_ALWAYS);
    
    shaderProgram->Use();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    SetSource();
    
    glUniform1i(shaderProgram->UniformLocation("tDiffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    
    glUniform1i(shaderProgram->UniformLocation("tDepth"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    
    glBindVertexArray(rectangle->VertexArray());
    
    glDrawElements(GL_TRIANGLES, rectangle->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(oldDepthFunctionMode);
}
