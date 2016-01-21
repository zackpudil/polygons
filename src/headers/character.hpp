#pragma once

#include <main.hpp>
#include <anim.hpp>
#include <model.hpp>
#include <shader.hpp>
#include <functional>
#include <GLFW/glfw3.h>

class Character {
public:
    glm::vec3 position;
    float angle = 0;
    float speed = 0;
    
    const std::function<Animator*(Animator*)> noop = [](Animator* a) -> Animator* { return a; };
    const float maxSpeed, angluarSpeed, scale;
    
    std::function<Animator*(Animator*)> idle = noop;
    std::function<Animator*(Animator*)> idleToWalk = noop;
    
    std::function<Animator*(Animator*)> walk = noop;
    std::vector<std::function<Animator*(Animator*)>> walkToIdles = {noop};
    
    Animator* animator;
    
    Character(Shader*,
              Shader*,
              Animator*,
              Model*,
              glm::vec3,
              const float maxSpeed = 2.0f,
              const float angularSpeed = 2.0f,
              const float scale = 1);
    
    void handleInput(GLFWwindow *);
    void draw(glm::mat4, glm::mat4);
    void drawOutline(glm::mat4, glm::mat4);
    void drawShadow(std::vector<glm::mat4>, glm::vec3, float);
    
    void emulateStop();
    void emulateGo();
    
    Ellipsoid getEllipsoid();
private:
    Shader* _shader;
    Shader* _shadowShader;
    Model* _model;
    
    bool _keyPressed = false;
    
    glm::vec3 _direction;
    
    glm::mat4 _getModelMatrix(float scaleAddon = 0.0f);
};