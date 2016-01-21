#include <thirdPersonCamera.hpp>

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 p, GLFWwindow *w, float s) :
    position(p),
    window(w),
    sensitity(s) { }


void ThirdPersonCamera::handleInput(glm::vec3 target) {
    glm::vec3 yDis = glm::vec3(0, 16, 0);
    double xpos, ypos;
    
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if(firstMove) {
        _lastXpos = xpos;
        _lastYpos = ypos;
        firstMove = false;
    }
    
    glm::vec3 focus = glm::normalize(position - (target + yDis));
    
    float yaw = (_lastXpos - xpos)*sensitity;
    float pitch = (_lastYpos - ypos)*sensitity;
    
    _lastXpos = xpos;
    _lastYpos = ypos;
    
    glm::vec3 right = glm::normalize(glm::cross(focus, glm::vec3(0, 1, 0)));
    glm::vec3 up = glm::normalize(glm::cross(right, focus));
    
    glm::mat4 roationMat;
    roationMat = glm::rotate(roationMat, glm::radians(yaw), up);
    roationMat = glm::rotate(roationMat, glm::radians(pitch), right);
    
    rotation = glm::vec3(roationMat*glm::vec4(focus, 1));
    
    position = target + yDis + 30.0f*rotation;
}

glm::mat4 ThirdPersonCamera::getViewMatrix(glm::vec3 target) {
    return glm::lookAt(position, target + glm::vec3(0, 16, 0), glm::vec3(0, 1, 0));
}