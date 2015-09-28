/*
Adjust color fragment shader.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tDepth;
uniform vec2 screenSize;

uniform vec3 color;

out vec4 fragmentColor;

// Calculate texcoord
vec2 calculateTexCoord() {
	return gl_FragCoord.xy / screenSize;
}

void main () {
	vec2 texCoord = calculateTexCoord();
	fragmentColor = texture(tDiffuse, texCoord) * vec4(color, 1.0);

	float depth = texture(tDepth, texCoord).r;
	gl_FragDepth = depth;
}