#include <character.hpp>

Character::Character(Shader* s,
                     Shader* ss,
                     Animator* a,
                     Model* m,
                     glm::vec3 p,
                     const float ms,
                     const float as,
                     const float sc) :
    position(p),
    maxSpeed(ms),
    angluarSpeed(as),
    scale(sc),
    animator(a),
    _shader(s),
    _shadowShader(ss),
    _model(m)
{ }

void Character::handleInput(GLFWwindow *window) {
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        angle += angluarSpeed;
    else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        angle -= angluarSpeed;
    
    _direction = glm::normalize(glm::vec3(
                                       cos(glm::radians(angle)),
                                       0,
                                       -sin(glm::radians(angle))));
    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        _keyPressed = true;
        emulateGo();
    } else {
        if(!_keyPressed)
            idle(animator);
        else
            emulateStop();
    }
}

void Character::emulateStop() {
    for(auto walkToIdle : walkToIdles) {
        walkToIdle(animator)->doWhile([this] {
            position += speed*_direction;
            speed -= 0.01f;
            speed = fmax(speed, 0);
        });
    }
    
    idle(animator)->doWhile([this] { speed = 0; });
}

void Character::emulateGo() {
    idleToWalk(animator)->doWhile([this] {
        position += speed*_direction;
        speed += 0.016f;
        speed = fmin(speed, maxSpeed);
    });
    
    walk(animator)->doWhile([this] {
        speed = maxSpeed;
        position += speed*_direction;
    });
 
}

void Character::draw(glm::mat4 projection, glm::mat4 view) {
    animator->run(_shader);
    
    glm::mat4 model = _getModelMatrix();
    
    _shader->bind("matrix.projection", projection)
        .bind("matrix.view", view)
        .bind("matrix.model", model)
        .bind("castShadow", false);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    _model->draw(_shader);
}

void Character::drawOutline(glm::mat4 projection, glm::mat4 view) {
    animator->get(_shader);
    glm::mat4 model = _getModelMatrix(0.0002f);
    
    _shader->bind("matrix.projection", projection)
        .bind("matrix.view", view)
        .bind("matrix.model", model);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    _model->draw(_shader, true, false);
    
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
}

void Character::drawShadow(std::vector<glm::mat4> shadowTrans, glm::vec3 lightPos, float far_plane) {
    animator->get(_shadowShader);
    glm::mat4 model = _getModelMatrix();
    
    _shadowShader->bind("model", model)
        .bind("lightPos", lightPos)
        .bind("far_plane", far_plane);
    
    for(uint i = 0; i < shadowTrans.size(); i++)
        _shadowShader->bind("shadowTransforms[" + std::to_string(i) + "]", shadowTrans[i]);
    
    _model->draw(_shadowShader, false);
}

Ellipsoid Character::getEllipsoid() {
    Ellipsoid ellipsoid = {
        position + glm::vec3(0, 5, 0),
        glm::mat3(1.0f/7.0f, 0, 0, 0, 1.0f/80.0f, 0, 0, 0, 1.0f/7.0f)
    };
    return ellipsoid;
}

glm::mat4 Character::_getModelMatrix(float scaleAddon) {
    glm::mat4 model;
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(scale + scaleAddon));
    
    return model;
}