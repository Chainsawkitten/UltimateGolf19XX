/*
Skybox - Fragment Shader
*/
#version 400
in vec3 texCoord;

uniform samplerCube cubeMapTexture;

out vec4 fragmentColor;

void main () {
	fragmentColor = texture(cubeMapTexture, texCoord);
}