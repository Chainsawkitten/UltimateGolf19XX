#pragma once

#include "Filter.hpp"

/** @ingroup Core
 * @{
 */

/// Fast approximate anti-aliasing filter.
class FXAAFilter : public Filter {
	public:
		/// Create new FXAA filter.
		FXAAFilter();

		/// Free allocated resources.
		~FXAAFilter();

		/// Get shader program.
		/**
		 * @return Shader program
		 */
		ShaderProgram* GetShaderProgram() const;

		/// Set uniforms.
		void SetUniforms();

	private:
		Shader* vertexShader;
		Shader* fragmentShader;
		ShaderProgram* shaderProgram;
};

/** @} */
