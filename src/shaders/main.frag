#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 color;

in vec4 out_color;
in vec2 out_texCoords;
in vec3 out_normal;
in vec3 fragPos;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform sampler2D tex;

void main() {
    //ambient light
    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * lightColor;
    
    // diffuse light
    vec3 norm = normalize(out_normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec4 diffuse = diff * lightColor;
    
    // specular light
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;
    
    vec4 result = (ambient + diffuse + specular);
    
    vec4 textel = texture(tex, out_texCoords);
    
    if(textel.xyz == vec3(0.0f)) {
        color = out_color*result;
    } else  if (out_color.w != 0.0f) {
        color = textel*out_color*result;
    } else {
        color = textel*result;
    }
}
