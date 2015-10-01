/*
Water fragment shader (reflection and refraction).
*/
#version 400

in VertexData {
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
    vec3 viewPosition;
} vertexIn;

uniform sampler2D tRefraction;
uniform sampler2D tReflection;
uniform sampler2D tDuDvMap;
uniform sampler2D tWater;

uniform vec2 screenSize;
uniform vec2 textureRepeat;

uniform vec2 texOffset;
const float waveStrength = 0.02;

out vec4 fragmentColor;

// Calculate texcoord
vec2 calculateTexCoord() {
    return gl_FragCoord.xy / screenSize;
}

void main() {
    vec2 texCoords = vertexIn.texCoords * textureRepeat + texOffset;
    vec2 distortion1 = (texture(tDuDvMap, texCoords).rg * 2.0 - 1.0) * waveStrength;
    
    vec2 refractionTexCoord = calculateTexCoord();
    vec2 reflectionTexCoord = refractionTexCoord;
    reflectionTexCoord.y = 1.0 - reflectionTexCoord.y;
    
    refractionTexCoord += distortion1;
    reflectionTexCoord += distortion1;
    
    vec4 refractionColor = texture(tRefraction, refractionTexCoord);
    vec4 reflectionColor = texture(tReflection, reflectionTexCoord);
    
	fragmentColor = mix(reflectionColor, refractionColor, dot(normalize(-vertexIn.viewPosition), normalize(vertexIn.normal)));
    fragmentColor = fragmentColor * mix(texture(tWater, texCoords), vec4(1.0, 1.0, 1.0, 1.0), 0.5);
}
