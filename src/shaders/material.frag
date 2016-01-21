#version 330 core

{{phong.glsl}}

struct Material {
    sampler2D diffuse[1];
    sampler2D specular[1];
    sampler2D normal[1];
    int shininess;
    
    bool hasNormals;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

out vec4 color;

uniform samplerCube shadowMap[1];

uniform vec3 viewPos;
uniform Material material;

uniform DirectionalLight directionals[5];
uniform int directionalLightAmount;

uniform SpotLight spots[1];
uniform int spotLightAmount;

uniform PointLight points[5];
uniform int pointLightAmount;

uniform float far_plane;
uniform bool castShadow;

vec3 getMaterialAmbient() { return vec3(texture(material.diffuse[0], TexCoords)); }
vec3 getMaterialDiffuse() { return vec3(texture(material.diffuse[0], TexCoords)); }
vec3 getMaterialSpecular() { return vec3(texture(material.specular[0], TexCoords)); }
float getMaterialShininess() { return material.shininess; }
vec3 getViewPos() { return viewPos; }

float getShadow(vec3 position, samplerCube map) {
    vec3 sampleOffsetDirections[20] = vec3[] (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );
    
    vec3 fragToLight = FragPos - position;
    float currentDepth = length(fragToLight);
    
    vec3 lightDir = normalize(fragToLight);
    
    float shadow = 0;
    float bias = 0.2;
    int samples = 20;
    float viewDistance = length(viewPos - FragPos);
    float diskRadius = (1.0 + (viewDistance/ far_plane))/1.5;
    
    for (int i = 0; i < samples; i++) {
        float closestDepth = texture(map, fragToLight+sampleOffsetDirections[i]*diskRadius).r;
        closestDepth *= far_plane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return castShadow ? shadow : 0;
}

void main() {
    vec3 norm;
    
    if(!material.hasNormals) {
        norm = normalize(Normal);
    } else {
        norm = texture(material.normal[0], TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(TBN * norm);
    }

    vec3 result;
    
    float shadows[5];
    
    for(int i = 0; i < pointLightAmount; i++)
        shadows[i] = getShadow(points[i].position, shadowMap[i]);
    
    for(int i = 0; i < directionalLightAmount; i++)
        result += calculateDirectionalLight(directionals[i], FragPos, norm);
    
    for (int i = 0; i < pointLightAmount; i++)
        result += points[i].ambient*getMaterialAmbient() + calculatePointLight(points[i], FragPos, norm)*(1.0 - shadows[i]);
    
    for(int i = 0; i < spotLightAmount; i++)
        result += calculateSpotLight(spots[i], FragPos, norm);
    
    color = vec4(result, 1.0f);
}