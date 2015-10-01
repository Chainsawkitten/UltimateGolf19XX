/*
Water fragment shader (reflection and refraction).
*/
#version 400

in VertexData {
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} vertexIn;

uniform sampler2D tRefraction;
uniform sampler2D tReflection;

uniform vec2 screenSize;

out vec4 fragmentColor;

// Calculate texcoord
vec2 calculateTexCoord() {
    return gl_FragCoord.xy / screenSize;
}

void main() {
    vec2 refractionTexCoord = calculateTexCoord();
    vec2 reflectionTexCoord = refractionTexCoord;
    reflectionTexCoord.y = 1.0 - reflectionTexCoord.y;
    
    vec4 refractionColor = texture(tRefraction, refractionTexCoord);
    vec4 reflectionColor = texture(tReflection, reflectionTexCoord);
    
	fragmentColor = mix(refractionColor, reflectionColor, 0.5);
}
