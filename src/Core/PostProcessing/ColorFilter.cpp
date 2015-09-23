#include "ColorFilter.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostColor.frag.hpp"

ColorFilter::ColorFilter(const glm::vec3& color) {
    vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(POSTCOLOR_FRAG, POSTCOLOR_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
	this->color = color;
}

ColorFilter::~ColorFilter() {
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
}

ShaderProgram* ColorFilter::GetShaderProgram() const {
	return shaderProgram;
}

void ColorFilter::SetUniforms() {
	glUniform3fv(shaderProgram->UniformLocation("color"), 1, &color[0]);
}

void ColorFilter::SetColor(const glm::vec3& color) {
	this->color = color;
}
