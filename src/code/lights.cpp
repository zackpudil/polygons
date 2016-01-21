#include <lights.hpp>

void Lights::light(std::vector<Shader *> shaders) {
    for(auto shader : shaders) {
        for(uint i = 0; i < _directionalLights.size(); i++) {
            DirectionalLight d = _directionalLights[i];
            std::string start = "directionals[" + std::to_string(i) + "].";
            
            shader->bind(start + "direction", d.direction);
            _bindCommonUniforms(d, shader, start);
        }
        shader->bind("directionalLightAmount", (int)_directionalLights.size());
        
        for(uint i = 0; i < _pointLights.size(); i++) {
            PointLight p = _pointLights[i];
            std::string start = "points[" + std::to_string(i) + "].";
            
            shader->bind(start + "position", p.position);
            shader->bind(start + "linear", p.linear);
            shader->bind(start + "quadratic", p.quadradic);
            
            _bindCommonUniforms(p, shader, start);
        }
        shader->bind("pointLightAmount", (int)_pointLights.size());
        
        for(uint i = 0; i < _spotLights.size(); i++) {
            SpotLight s = _spotLights[i];
            std::string start = "spots[" + std::to_string(i) + "].";
            
            shader->bind(start + "position", s.position);
            shader->bind(start + "direction", s.direction);
            shader->bind(start + "linear", s.linear);
            shader->bind(start + "quadratic", s.quadratic);
            shader->bind(start + "cutOff", s.innerCutOff);
            shader->bind(start + "outerCutOff", s.outerCutOff);
            
            _bindCommonUniforms(s, shader, start);
        }
        shader->bind("spotLightAmount", (int)_spotLights.size());
    }
}

void Lights::updateDirectionalLights(std::vector<glm::vec3> directions) {
    for(uint i = 0; i < _directionalLights.size(); i++) {
        if(i < directions.size())
            _directionalLights[i].direction = directions[i];
    }
}

void Lights::updatePointLights(std::vector<glm::vec3> positiions) {
    for(uint i = 0; i < _pointLights.size(); i++) {
        if(i < positiions.size())
            _pointLights[i].position = positiions[i];
    }
}

void Lights::updateSpotLights(std::vector<std::pair<glm::vec3, glm::vec3>> positionAndDirections) {
    for(uint i = 0; i < _spotLights.size(); i++) {
        if(i < positionAndDirections.size()) {
            _spotLights[i].position = positionAndDirections[i].first;
            _spotLights[i].direction = positionAndDirections[i].second;
        }
    }
}

Lights& Lights::addDirectionalLight(glm::vec3 dir, glm::vec3 a, glm::vec3 d, glm::vec3 s) {
    _directionalLights.push_back(DirectionalLight(dir, a, d, s));
    return *this;
}


Lights& Lights::addPointLight(glm::vec3 p, float l, float q, glm::vec3 a, glm::vec3 d, glm::vec3 s) {
    _pointLights.push_back(PointLight(p, l, q, a, d, s));
    return *this;
}

Lights& Lights::addSpotLight(glm::vec3 p, glm::vec3 dir,
                             float l, float q, float i, float o, glm::vec3 a, glm::vec3 d, glm::vec3 s)
{
    _spotLights.push_back(SpotLight(p, dir, l, q, i, o, a, d, s));
    return *this;
}

void Lights::_bindCommonUniforms(Light l, Shader *s, std::string start) {
    s->bind(start + "ambient", l.ambient)
        .bind(start + "diffuse", l.diffuse)
        .bind(start + "specular", l.specular);
}