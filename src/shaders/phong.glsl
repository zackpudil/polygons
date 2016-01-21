vec3 getMaterialAmbient();
vec3 getMaterialDiffuse();
vec3 getMaterialSpecular();
float getMaterialShininess();

vec3 getViewPos();

vec3 calculateDiffuse(vec3 diffuse, vec3 lightDirection, vec3 norm) {
    float diff = max(dot(norm, lightDirection), 0.0);
    return diffuse * diff;
}

vec3 calculateSpecular(vec3 specular, vec3 lightDirection, vec3 fragPos, vec3 norm) {
    vec3 viewDir = normalize(getViewPos() - fragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), getMaterialShininess());
    return specular * spec;
}

float calculateLuminosity(vec3 pos, vec3 fragPos, float l, float q) {
    float d  = length(pos - fragPos);
    float attenuation = q*d*d;
    return 1.0f/(attenuation);
}

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calculateDirectionalLight(DirectionalLight directional, vec3 fragPos, vec3 norm) {
    
    vec3 lightDir = normalize(-directional.direction);
    
    vec3 ambient = directional.ambient*getMaterialAmbient();
    vec3 diffuse = calculateDiffuse(directional.diffuse, lightDir, norm)*getMaterialDiffuse();
    vec3 specular = calculateSpecular(directional.specular, lightDir, fragPos, norm)*getMaterialSpecular();
    
    return diffuse + specular;
}

struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float linear;
    float quadratic;
};

vec3 calculatePointLight(PointLight point, vec3 fragPos, vec3 norm) {
    vec3 lightDir = normalize(point.position - fragPos);
    
    float a = calculateLuminosity(point.position, fragPos, point.linear, point.quadratic);
    
    vec3 ambient = point.ambient*getMaterialAmbient();
    vec3 diffuse = calculateDiffuse(point.diffuse, lightDir, norm)*a*getMaterialDiffuse();
    vec3 specular = calculateSpecular(point.specular, lightDir, fragPos, norm)*a*getMaterialSpecular();
    
    return (ambient + (diffuse + specular));
}

struct SpotLight {
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float linear;
    float quadratic;
    
    float cutOff;
    float outerCutOff;
};

vec3 calculateSpotLight(SpotLight spot, vec3 fragPos, vec3 norm) {
    vec3 lightDir = normalize(spot.position - fragPos);
    
    float theta = dot(lightDir, normalize(-spot.direction));
    float epsilon = (spot.cutOff - spot.outerCutOff);
    float intensity = clamp((theta - spot.outerCutOff)/epsilon, 0.0, 1.0);
    
    float a = calculateLuminosity(spot.position, fragPos, spot.linear, spot.quadratic);
    
    vec3 diffuse = calculateDiffuse(spot.diffuse, lightDir, norm)*a*getMaterialDiffuse();
    vec3 specular = calculateSpecular(spot.specular, lightDir, fragPos, norm)*a*getMaterialSpecular();
    
    return (spot.ambient + (diffuse + specular))*getMaterialDiffuse();
}