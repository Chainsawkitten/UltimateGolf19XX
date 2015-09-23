/*
Skybox - Vertex Shader
*/
#version 400
layout(location = 0) in vec3 vertexPosition;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 texCoord;

void main () {
	vec4 VPPosition = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0);
	gl_Position = VPPosition.xyww;
	texCoord = vertexPosition;
}