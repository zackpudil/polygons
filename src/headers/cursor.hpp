#pragma once

#include <main.hpp>
#include <shader.hpp>

class Cursor {
public:
    float width;
    float height;
    glm::vec3 color;
    float cursorLength;
    
    Cursor(float, float, glm::vec3, float l = 7);
    
    void draw();
    
private:
    Shader _shader;
    glm::vec2 _center;
    
    GLuint _vao;
    
    std::vector<float> _shape;
    
    void _createVertexAttribArray();
};