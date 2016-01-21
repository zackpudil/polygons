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
layout (location = 4) in ivec4 boneIds;
layout (location = 5) in vec4 weights;
layout (location = 6) in int boneAmount;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
out mat3 TBN;

uniform ModelViewProjMatrix matrix;
uniform mat4 bones[20];

void main() {
    mat4 boneTransform;
    
    for(int i = 0; i < boneAmount; i++)
        boneTransform += bones[boneIds[i]] * weights[i];
    
    vec3 T = normalize(vec3(matrix.model*boneTransform*vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(matrix.model*boneTransform*vec4(normal, 0.0)));
    vec3 B = normalize(cross(T, N));
    
    TBN = mat3(T, B, N);
    
    vec4 p = boneTransform * vec4(position, 1.0f);
    
    gl_Position = matrix.projection * matrix.view * matrix.model * p;
    
    FragPos = vec3(matrix.model * p);
    Normal = mat3(transpose(inverse(matrix.model*boneTransform)))*normal;
    TexCoords = tex;
}
