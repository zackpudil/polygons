#version 330 core

layout (location = 0) in vec3 position;
layout (location = 4) in ivec4 boneIds;
layout (location = 5) in vec4 weights;
layout (location = 6) in int boneAmount;

uniform mat4 model;
uniform mat4 bones[20];

void main() {
    mat4 boneTransform;
    
    for(int i = 0; i < boneAmount; i++)
        boneTransform += bones[boneIds[i]] * weights[i];
    
    gl_Position = model * boneTransform * vec4(position, 1.0f);
}