/*
Simple pass-through vertex shader - Vertex Shader
*/
#version 400
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexture;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

out VertexData {
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} vertexOut;

void main () {
	vertexOut.normal = vertexNormal;
	vertexOut.tangent = vertexTangent;
	vertexOut.texCoords = vertexTexture;
	gl_Position = vec4(vertexPosition, 1.0);
}