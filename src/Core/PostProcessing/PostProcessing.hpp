#pragma once

#include "Filter.hpp"
#include <glm/glm.hpp>

/** @ingroup Core
 * @{
 */

/// Applies post-processing effects to the rendered image.
class PostProcessing {
	public:
		/// Create new post-processing handler.
		/**
		 * @param size Size of the context.
		 */
		PostProcessing(const glm::vec2& size);

		/// Free allocated resources.
		~PostProcessing();

		/// Set as render target.
		void SetTarget();

		/// Apply a filter to the render image.
		/**
		 * filter %Filter to apply.
		 */
		void ApplyFilter(Filter* filter);

		/// Render resulting image to screen.
		void Render();

	private:
        void BindQuad();
        
		unsigned int width;
		unsigned int height;

		short int which = 0;

		GLuint frameBuffer[2];
		GLuint bufferTexture[2];
		GLuint depthBuffer[2];

		// Full screen quad
		static const glm::vec2 vertices[4];
		static const unsigned int indices[6];

		// Vertex buffer
		GLuint vertexBuffer;
		GLuint vertexAttribute;
		unsigned int vertexCount;

		// Index buffer
		GLuint indexBuffer;
		unsigned int indexCount;

		// Shaders
		Shader* vertexShader;
		Shader* fragmentShader;
		ShaderProgram* shaderProgram;
};

/** @} */
