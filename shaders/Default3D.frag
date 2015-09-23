/*
Geometry pass fragment shader (first pass)
*/
#version 400
in VertexData {
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} vertexIn;

uniform sampler2D baseImage;

layout(location = 0) out vec3 diffuseOut;
layout(location = 1) out vec3 normalsOut;
layout(location = 2) out vec3 specularOut;

void main() {
	diffuseOut = texture(baseImage, vertexIn.texCoords).rgb;
	normalsOut = vertexIn.normal;
	specularOut = vec3(1.0, 1.0, 1.0);
}