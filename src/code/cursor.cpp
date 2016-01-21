#include <cursor.hpp>

Cursor::Cursor(float w, float h, glm::vec3 c, float l) :
    width(w),
    height(h),
    color(c),
    cursorLength(l)
{
    _shader
        .attach("ortho.vert")
        .attach("ortho.frag")
        .link();
    
    _center = glm::vec2(width/2, height/2);
    
    _shape = {
        _center.x, _center.y-cursorLength, _center.x, _center.y+cursorLength,
        _center.x-cursorLength, _center.y, _center.x+cursorLength, _center.y
    };
    
    _createVertexAttribArray();
}

void Cursor::draw() {
    glm::mat4 ortho = glm::ortho(0.0, (double)width, 0.0, (double)height);
    
    _shader.activate().bind("projection", ortho);
    
    glBindVertexArray(_vao);
    glDrawArrays(GL_LINES, 0, _shape.size());
    glBindVertexArray(0);
}

void Cursor::_createVertexAttribArray() {
    GLuint vbo;
    
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, _shape.size()*sizeof(float), &_shape[0], GL_STATIC_DRAW);
    
    glBindVertexArray(_vao);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}