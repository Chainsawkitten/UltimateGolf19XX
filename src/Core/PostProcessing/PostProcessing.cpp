#include "PostProcessing.hpp"
#include "../Util/Log.hpp"
#include "../Resources.hpp"

PostProcessing::PostProcessing(const glm::vec2& size) {
    buffers[0] = new RenderTarget(size);
    buffers[1] = new RenderTarget(size);
    
    rectangle = Resources().CreateRectangle();
    
    which = 0;
}

PostProcessing::~PostProcessing() {
    delete buffers[0];
    delete buffers[1];
    
    Resources().FreeRectangle();
}

RenderTarget* PostProcessing::GetRenderTarget() const {
    return buffers[which];
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
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    buffers[which]->Render();
}
