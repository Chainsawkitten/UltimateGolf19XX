#include "PostProcessing.hpp"
#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostCopy.frag.hpp"

PostProcessing::PostProcessing(const glm::vec2& size) {
    width = static_cast<int>(size.x);
    height = static_cast<int>(size.y);
    
    // Textures
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(2, bufferTexture);
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, bufferTexture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Depth buffers
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(2, depthBuffer);
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, depthBuffer[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Framebuffer objects
    glGenFramebuffers(2, frameBuffer);
    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTexture[i], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer[i], 0);
    }
    
    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Frame Buffer failed\n";
    
    // Default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    rectangle = Resources().CreateRectangle();
    
    vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(POSTCOPY_FRAG, POSTCOPY_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
}

PostProcessing::~PostProcessing() {
    glDeleteTextures(2, depthBuffer);
    glDeleteTextures(2, bufferTexture);
    glDeleteFramebuffers(2, frameBuffer);
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    Resources().FreeRectangle();
}

void PostProcessing::SetTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer[0]);
    which = 0;
}

void PostProcessing::ApplyFilter(Filter* filter) {
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    
    GLint oldDepthFunctionMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunctionMode);
    glDepthFunc(GL_ALWAYS);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer[1 - which]);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer[which]);
    
    filter->GetShaderProgram()->Use();
    
    glm::vec2 screenSize(width, height);
    glUniform2fv(filter->GetShaderProgram()->UniformLocation("screenSize"), 1, &screenSize[0]);
    
    glUniform1i(filter->GetShaderProgram()->UniformLocation("tDiffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferTexture[which]);
    
    glUniform1i(filter->GetShaderProgram()->UniformLocation("tDepth"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthBuffer[which]);
    
    glBindVertexArray(rectangle->VertexArray());
    
    filter->SetUniforms();
    
    glDrawElements(GL_TRIANGLES, rectangle->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(oldDepthFunctionMode);
    
    which = 1 - which;
}

void PostProcessing::Render() {
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    
    GLint oldDepthFunctionMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunctionMode);
    glDepthFunc(GL_ALWAYS);
    
    shaderProgram->Use();
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer[which]);
    
    glm::vec2 screenSize(width, height);
    glUniform2fv(shaderProgram->UniformLocation("screenSize"), 1, &screenSize[0]);
    
    glUniform1i(shaderProgram->UniformLocation("tDiffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferTexture[which]);
    
    glUniform1i(shaderProgram->UniformLocation("tDepth"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthBuffer[which]);
    
    glBindVertexArray(rectangle->VertexArray());
    
    glDrawElements(GL_TRIANGLES, rectangle->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(oldDepthFunctionMode);
}
