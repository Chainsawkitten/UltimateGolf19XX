#include "FXAAFilter.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostFXAA.frag.hpp"

FXAAFilter::FXAAFilter() {
    vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(POSTFXAA_FRAG, POSTFXAA_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
}

FXAAFilter::~FXAAFilter() {
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
}

ShaderProgram* FXAAFilter::GetShaderProgram() const {
	return shaderProgram;
}

void FXAAFilter::SetUniforms() {
	// FXAA shader doesn't need any special uniforms.
}
