/*
Water fragment shader (reflection and refraction).
*/
#version 400
in VertexData {
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} vertexIn;

out vec4 fragmentColor;

void main() {
	fragmentColor = vec4(0.0, 0.0, 1.0, 1.0);
}
