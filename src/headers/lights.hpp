#pragma once

#include <main.hpp>
#include <shader.hpp>

enum LightType {
    directional,
    point,
    spot
};

struct Light {
    LightType type;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    Light(LightType t, glm::vec3 a, glm::vec3 d, glm::vec3 s) :
        type(t),
        ambient(a),
        diffuse(d),
        specular(s) { }
};

struct DirectionalLight : Light {
    glm::vec3 direction;
    
    DirectionalLight(glm::vec3 dir, glm::vec3 a, glm::vec3 d, glm::vec3 s) :
        Light(directional, a, d, s),
        direction(dir) { }
        
};

struct PointLight : Light {
    glm::vec3 position;
    
    float linear;
    float quadradic;
    
    GLuint shadowBufferFrame;
    GLuint shadowCubeMap;
    
    
    PointLight(glm::vec3 p, float l, float q, glm::vec3 a, glm::vec3 d, glm::vec3 s) :
        Light(point, a, d, s),
        position(p),
        linear(l),
        quadradic(q) { }
    
    std::vector<glm::mat4> getTransforms(float f) {
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, f);
        std::vector<glm::mat4> shadowViews;
        std::vector<glm::mat4> shadowTransforms;
        glm::vec3 p = this->position;
        
        shadowViews.push_back(glm::lookAt(p, p + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
        shadowViews.push_back(glm::lookAt(p, p + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
        shadowViews.push_back(glm::lookAt(p, p + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
        shadowViews.push_back(glm::lookAt(p, p + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
        shadowViews.push_back(glm::lookAt(p, p + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
        shadowViews.push_back(glm::lookAt(p, p + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));
        
        shadowTransforms.resize(shadowViews.size());
        std::transform(shadowViews.begin(),
                       shadowViews.end(),
                       shadowTransforms.begin(), [&](glm::mat4 v) {
                           return shadowProj * v;
                       });
        
        return shadowTransforms;
    }
};

struct SpotLight : Light {
    glm::vec3 direction;
    glm::vec3 position;
    
    float linear;
    float quadratic;
    
    float innerCutOff;
    float outerCutOff;
    
    SpotLight(glm::vec3 p, glm::vec3 dir,
              float l, float q, float i, float o, glm::vec3 a, glm::vec3 d, glm::vec3 s) :
        Light(spot, a, d, s),
        direction(dir),
        position(p),
        linear(l),
        quadratic(q),
        innerCutOff(i),
        outerCutOff(o) { }
};

class Lights {
public:
    std::vector<DirectionalLight> _directionalLights;
    std::vector<PointLight> _pointLights;
    std::vector<SpotLight> _spotLights;
    
    void light(std::vector<Shader*>);
    
    void updateDirectionalLights(std::vector<glm::vec3>);
    void updatePointLights(std::vector<glm::vec3>);
    void updateSpotLights(std::vector<std::pair<glm::vec3, glm::vec3>>);
    
    Lights& addDirectionalLight(glm::vec3, glm::vec3, glm::vec3, glm::vec3);
    Lights& addPointLight(glm::vec3, float, float, glm::vec3, glm::vec3, glm::vec3);
    Lights& addSpotLight(glm::vec3, glm::vec3, float, float, float, float, glm::vec3, glm::vec3, glm::vec3);
private:
    void _bindCommonUniforms(Light, Shader*, std::string);
};