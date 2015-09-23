#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Light.hpp"

#include "Shader/ShaderProgram.hpp"
#include "Camera/Camera.hpp"

/** @ingroup Core
 * @{
 */

/// Holds the frame buffers used for deferred rendering.
class RenderTarget {
	public:
		/// The different buffer types.
		enum TEXTURE_TYPE {
			DIFFUSE, ///< Diffuse texture
			NORMAL, ///< Normals
			SPECULAR, ///< Specular
			NUM_TEXTURES ///< Total number of textures (excluding depth buffer)
		};
        
		/// Create new render target.
		/**
		 * @param size Size of the context.
		 */
		RenderTarget(const glm::vec2& size);
        
		/// Destructor
		/**
		 * Free allocated resources.
		 */
		~RenderTarget();
        
		/// Get texture.
		/**
		 * @param textureType The type of texture to get.
		 * @return The texture identifier
		 */
		GLuint Texture(TEXTURE_TYPE textureType) const;
        
		/// Enable frame buffer object for writing.
		void BindForWriting();
        
		/// Enable frame buffer object for reading.
		void BindForReading();
        
		/// Enable frame buffer object for reading textures.
		void BindForTexReading();
        
		/// Set buffer to read from.
		/**
		 * @param textureType Texture type of the buffer to read from.
		 */
		void SetReadBuffer(TEXTURE_TYPE textureType);
        
		/// Render the content of diffuse, position and normal textures.
		/**
		 * @param screenSize Size of the screen in pixels.
		 */
		void ShowTextures(const glm::vec2& screenSize);
        
		/// Resets the render target.
		/**
		 * Needs to be called before rendering to the screen.
		 */
		void ResetWriting();
        
		/// Render the lighting in the scene.
		/**
		 * @param camera Camera to use.
		 * @param screenSize Size of the screen in pixels.
		 */
		void Render(Camera* camera, const glm::vec2& screenSize);
        
		/// Light.
		Light light;
        
	private:
        static void AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat);
		void BindLighting(Camera* camera, const glm::vec2& screenSize);
		void BindQuad();
        
		GLuint textures[NUM_TEXTURES];
        
		GLuint fbo;
		GLuint depthHandle;
        
        Shader* vertexShader;
        Shader* fragmentShader;
		ShaderProgram* shaderProgram;
        
		glm::vec2 size;
        
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
};

/** @} */