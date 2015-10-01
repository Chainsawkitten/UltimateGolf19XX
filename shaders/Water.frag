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

uniform vec4 lightPosition;
uniform vec3 lightIntensity;

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
    
    float refractiveFactor = sqrt(dot(normalize(-vertexIn.viewPosition), normalize(vertexIn.normal)));
    
	fragmentColor = mix(reflectionColor, refractionColor, refractiveFactor);
    fragmentColor = fragmentColor * mix(texture(tWater, texCoords), vec4(1.0, 1.0, 1.0, 1.0), 0.5);
    
    float shinyPower = 20.0;
    vec3 v = normalize(-vertexIn.viewPosition);
    vec3 lightDirection = normalize(vec3(lightPosition) - vertexIn.viewPosition);
	vec3 r = normalize(reflect(-lightDirection, vertexIn.normal));
    vec3 specularLight = vec3(1.0, 1.0, 1.0) * pow(max(dot(r, v), 0.0), shinyPower);
	fragmentColor = vec4(lightIntensity * (fragmentColor.rgb + specularLight), 1.0);
}
