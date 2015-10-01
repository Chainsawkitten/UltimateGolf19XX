#include "PostProcessing.hpp"
#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostCopy.frag.hpp"

PostProcessing::PostProcessing(const glm::vec2& size) {
    buffers[0] = new RenderTarget(size);
    buffers[1] = new RenderTarget(size);
    
    rectangle = Resources().CreateRectangle();
    
    vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(POSTCOPY_FRAG, POSTCOPY_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    
    which = 0;
}

PostProcessing::~PostProcessing() {
    delete buffers[0];
    delete buffers[1];
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    Resources().FreeRectangle();
}

void PostProcessing::SetTarget() {
    buffers[0]->SetTarget();
    which = 0;
}

void PostProcessing::ApplyFilter(Filter* filter) {
    // Disable depth testing
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    
    GLint oldDepthFunctionMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunctionMode);
    glDepthFunc(GL_ALWAYS);
    
    buffers[1 - which]->SetTarget();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    buffers[which]->SetSource();
    
    filter->GetShaderProgram()->Use();
    
    glUniform2fv(filter->GetShaderProgram()->UniformLocation("screenSize"), 1, &buffers[0]->Size()[0]);
    
    glUniform1i(filter->GetShaderProgram()->UniformLocation("tDiffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffers[which]->ColorTexture());
    
    glUniform1i(filter->GetShaderProgram()->UniformLocation("tDepth"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, buffers[which]->DepthTexture());
    
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
    
    buffers[which]->SetSource();
    
    glUniform2fv(shaderProgram->UniformLocation("screenSize"), 1, &buffers[0]->Size()[0]);
    
    glUniform1i(shaderProgram->UniformLocation("tDiffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffers[which]->ColorTexture());
    
    glUniform1i(shaderProgram->UniformLocation("tDepth"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, buffers[which]->DepthTexture());
    
    glBindVertexArray(rectangle->VertexArray());
    
    glDrawElements(GL_TRIANGLES, rectangle->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    
    glDepthFunc(oldDepthFunctionMode);
}
