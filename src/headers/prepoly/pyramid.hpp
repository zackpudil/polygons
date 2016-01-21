#pragma once

#include <main.hpp>
#include <polygon.hpp>


const std::vector<glm::vec3> pyramidShape = {
    glm::vec3( 0.5f, -0.5f, -0.5f), //0 back right
    glm::vec3( 0.5f, -0.5f,  0.5f), //1 front right
    glm::vec3(-0.5f, -0.5f,  0.5f), //2 front left
    glm::vec3(-0.5f, -0.5f, -0.5f), //3 back left
    
    glm::vec3(0.0f, 0.5f, 0.0f) // top
};

const std::vector<glm::vec2> pyramidUVMapping= {
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
};

const std::vector<unsigned int> pyramidIndices = {
    0, 1, 3, 1, 2, 3,
    
    0, 4, 1,
    1, 4, 2,
    2, 4, 3,
    3, 4, 0
};

class Pyramid : public Polygon {
public:
    Pyramid(glm::vec3 p) :
        Polygon(p, glm::vec4(0.0f), pyramidShape, pyramidUVMapping, pyramidIndices) { }
    
    Pyramid(glm::vec3 p, glm::vec4 c) :
       Polygon(p, c, pyramidShape, pyramidUVMapping, pyramidIndices) { }
};