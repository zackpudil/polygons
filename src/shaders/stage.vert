#version 330 core

struct ModelViewProjMatrix {
    mat4 projection;
    mat4 view;
    mat4 model;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 tangent;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
out mat3 TBN;

uniform ModelViewProjMatrix matrix;

void main() {
    vec4 p = vec4(position, 1.0f);
    
    vec3 T = normalize(vec3(matrix.model*vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(matrix.model*vec4(normal, 0.0)));
    vec3 B = normalize(cross(T, N));
    
    TBN = mat3(T, B, N);
    
    gl_Position = matrix.projection * matrix.view * matrix.model * p;
    
    FragPos = vec3(matrix.model * p);
    Normal = mat3(transpose(inverse(matrix.model)))*normal;
    TexCoords = tex;
}
