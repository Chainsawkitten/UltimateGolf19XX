/*
Lighting pass vertex shader (second pass)
*/
#version 400

layout(location = 0) in vec2 vertexPosition;

void main () {
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
}