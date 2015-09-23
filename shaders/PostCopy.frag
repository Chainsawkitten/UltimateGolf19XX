/*
Copy fragment shader.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tDepth;
uniform vec2 screenSize;

out vec4 fragmentColor;

// Calculate texcoord
vec2 calculateTexCoord() {
	return gl_FragCoord.xy / screenSize;
}

void main () {
	vec2 texCoord = calculateTexCoord();
	fragmentColor = texture(tDiffuse, texCoord);

	float depth = texture(tDepth, texCoord).r;
	gl_FragDepth = depth;
}