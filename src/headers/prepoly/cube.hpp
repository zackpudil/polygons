#pragma once

#include <main.hpp>
#include <polygon.hpp>

const std::vector<glm::vec3> cubeShape = {
    glm::vec3( 0.5f,  0.5f, -0.5f), //back top right 0
    glm::vec3( 0.5f, -0.5f, -0.5f), //back bottom right 1
    glm::vec3(-0.5f, -0.5f, -0.5f), //back bottom left 2
    glm::vec3(-0.5f,  0.5f, -0.5f), //back top left 3
    
    glm::vec3( 0.5f,  0.5f,  0.5f), //front top right 4
    glm::vec3( 0.5f, -0.5f,  0.5f), //front bottom right 5
    glm::vec3(-0.5f, -0.5f,  0.5f), //front bottom left 6
    glm::vec3(-0.5f,  0.5f,  0.5f)  //front top left 7
};


const std::vector<glm::vec2> cubeUVMap = {
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, 1.0f)
};

const std::vector<unsigned int> cubeIndices = {
    //back face
    0, 1, 3, 1, 2, 3,
    // front face
    7, 6, 4, 6, 5, 4,
    // left face
    3, 2, 7, 2, 6, 7,
    // right face
    4, 5, 0, 5, 1, 0,
    // top face
    3, 7, 0, 7, 4, 0,
    // bottom face
    1, 5, 2, 5, 6, 2
};

class Cube : public Polygon {
    
public:
    
    Cube(glm::vec3 p) :
        Polygon(p, glm::vec4(0.0f), cubeShape, cubeUVMap, cubeIndices) { }
    
    Cube(glm::vec3 p, glm::vec4 c) :
       Polygon(p, c, cubeShape, cubeUVMap, cubeIndices) { }
};