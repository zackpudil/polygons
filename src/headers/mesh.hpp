#pragma once

#include <main.hpp>
#include <shader.hpp>
#include <sstream>
#include <pipeline.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex;
    glm::vec3 tangent;
    
    int boneIds[10];
    float weights[10];
    int boneAndWeightSize = 0;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    int shininess;
    
    Mesh(std::vector<Vertex> v, std::vector<GLuint> i, std::vector<Texture> t, int s) :
        vertices(v),
        indices(i),
        textures(t),
        shininess(s) { this->_setupMesh(); }
    
    void draw(Shader* shader, bool m, bool s);
    
private:
    
    GLuint _vao, _vbo, _ebo;
    void _setupMesh();
};