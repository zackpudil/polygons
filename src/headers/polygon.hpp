#pragma once

#include <main.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <shader.hpp>
#include <pipeline.hpp>

#include <material.hpp>


class Polygon {
public:
    
    glm::vec3 position;
    glm::vec4 color;
    
    Shader* shader;
    
    Material material;
    
    Polygon(glm::vec3, glm::vec4,
            std::vector<glm::vec3>,
            std::vector<glm::vec2>,
            std::vector<unsigned int>);
    
    Polygon(glm::vec3, glm::vec4,
            std::vector<glm::vec3>,
            std::vector<glm::vec3>,
            std::vector<glm::vec2>,
            std::vector<unsigned int>);
    
    
    void init(Shader *shader);
    
    void translate(glm::vec3);
    void rotate(float, glm::vec3);
    void orbit(float, glm::vec3, glm::vec3);
    void scale(float);
    
    void addTexture(std::string const &, bool);
    
    void draw(glm::mat4, glm::mat4);
    
    glm::vec3 getPosition();
    
private:
    GLuint _vao;
    GLuint _textureId = 0;
    std::vector<float> _verts;
    
    glm::mat4 _translation;
    glm::mat4 _scale;
    glm::mat4 _rotation;
    glm::mat4 _orbit;
    
    void _updateUniforms(glm::mat4, glm::mat4);
};