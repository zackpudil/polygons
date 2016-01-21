#pragma once

#include <main.hpp>
#include <lights.hpp>
#include <model.hpp>
#include <util.hpp>
#include <shader.hpp>

class Stage {
public:
    const float scale;
    
    Stage(std::string, Shader*, Shader*, const float s = 1.0f);
    void draw(glm::mat4, glm::mat4);
    void drawShadow(std::vector<glm::mat4>, glm::vec3, float);
    
    Lights lights;
private:
    Model _model;
    Shader* _shader;
    Shader* _shadowShader;
    
    void _processLight(aiLight* light);
    glm::vec3 _findAmbient();
    
    glm::mat4 _getModelMatrix();
};