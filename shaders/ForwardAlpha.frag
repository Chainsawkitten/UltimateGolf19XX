/*
Forward rendering with alpha.
*/
#version 400
in VertexData {
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} vertexIn;

uniform sampler2D baseImage;

out vec4 fragmentColor;

void main() {
	fragmentColor = texture(baseImage, vertexIn.texCoords);
}
