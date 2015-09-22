#include "Skybox.hpp"
#include "Skybox.vert.hpp"
#include "Skybox.frag.hpp"
#include "Resources.hpp"

Skybox::Vertex Skybox::vertices[8] = { {-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {-1.f, -1.f, 1.f}, {1.f, -1.f, 1.f}, {-1.f, 1.f, -1.f}, {1.f, 1.f, -1.f}, {-1.f, -1.f, -1.f}, {1.f, -1.f, -1.f} };
unsigned int Skybox::indices[36] = { 0, 1, 3, 0, 3, 2, 2, 3, 7, 2, 7, 6, 4, 7, 5, 4, 6, 7, 4, 5, 1, 4, 1, 0, 1, 5, 7, 1, 7, 3, 0, 6, 4, 0, 2, 6 };

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

Skybox::Skybox(const CubeMapTexture* texture) {
	this->texture = texture;
    
    vertexShader = Resources().CreateShader(SKYBOX_VERT, SKYBOX_VERT_LENGTH, GL_VERTEX_SHADER);
    fragmentShader = Resources().CreateShader(SKYBOX_FRAG, SKYBOX_FRAG_LENGTH, GL_FRAGMENT_SHADER);
	shaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });

	BindData();
}

Skybox::~Skybox() {
	Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void Skybox::Render(int width, int height, const Camera* camera) {
	shaderProgram->Use();

	// Since our Z is always 1.0, we need to set depth buffer to render at <= 1.0 instead of the default < 1.0.
	GLint oldDepthFunctionMode;
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunctionMode);
	glDepthFunc(GL_LEQUAL);

	// Cube map texture.
	glUniform1i(shaderProgram->UniformLocation("cubeMapTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->TextureID());

	// Set uniforms.
	glUniformMatrix4fv(shaderProgram->UniformLocation("viewMatrix"), 1, GL_FALSE, &camera->Orientation()[0][0]);
	glUniformMatrix4fv(shaderProgram->UniformLocation("projectionMatrix"), 1, GL_FALSE, &camera->Projection(width, height)[0][0]);

	glBindVertexArray(vertexAttribute);

	// Render
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	glDepthFunc(oldDepthFunctionMode);
}

void Skybox::BindData() {
	// Vertex buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Skybox::Vertex), vertices, GL_STATIC_DRAW);

	// Define vertex data layout
	glGenVertexArrays(1, &vertexAttribute);
	glBindVertexArray(vertexAttribute);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Skybox::Vertex), BUFFER_OFFSET(0));

	// Index buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}
