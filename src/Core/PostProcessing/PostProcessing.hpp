#pragma once

#include "Filter.hpp"
#include "../Geometry/Rectangle.hpp"

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
		unsigned int width;
		unsigned int height;

		short int which = 0;

		GLuint frameBuffer[2];
		GLuint bufferTexture[2];
		GLuint depthBuffer[2];

		Geometry::Rectangle* rectangle;

		// Shaders
		Shader* vertexShader;
		Shader* fragmentShader;
		ShaderProgram* shaderProgram;
};

/** @} */
