#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 out_color;
out vec2 out_texCoords;
out vec3 out_normal;
out vec3 fragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 color;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    out_color = color;
    out_texCoords = texCoords;
    
    out_normal = normalize(mat3(transpose(inverse(model)))*normal);
    fragPos = vec3(model * vec4(position, 1.0f));
}
