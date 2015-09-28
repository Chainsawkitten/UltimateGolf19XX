#pragma once

#include <GL/glew.h>
#include "Texture/CubeMapTexture.hpp"
#include "Shader/ShaderProgram.hpp"
#include "Camera/Camera.hpp"

/** @ingroup Core
 * @{
 */

/// A skybox.
/**
 * A box rendered behind all other objects and which moves with the player, creating the illusion that it is infinitely far away.
 * Useful for making game worlds appear larger than they are.
 */
class Skybox {
	public:
		/// A vertex in the skybox.
		struct Vertex {
			/// X-position.
			float x;
			/// Y-position.
			float y;
			/// Z-position.
			float z;
		};

		/// Create new skybox from a cubemap texture.
		/**
		 * @param texture The cubemap texture to apply to the faces of the box.
		 */
		Skybox(const CubeMapTexture* texture);

		/// Destructor
		/**
		 * Free all allocated resources.
		 */
		~Skybox();

		/// Render skybox.
		/**
		 * @param camera Camera through which to render (only orientation is used, not translation).
		 * @param screenSize Size of the screen in pixels.
		 */
		void Render(const Camera* camera, const glm::vec2& screenSize);

	private:
		void BindData();

		static Vertex vertices[8];
		static unsigned int indices[36];

		const CubeMapTexture* texture;

		// Shaders
		Shader* vertexShader;
		Shader* fragmentShader;
		ShaderProgram* shaderProgram;

		// Vertex buffer
		GLuint vertexBuffer;
		GLuint vertexAttribute;

		// Index buffer.
		GLuint indexBuffer;
};

/** @} */
