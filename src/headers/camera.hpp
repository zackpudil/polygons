#pragma once

#include <character.hpp>
#include <main.hpp>
#include <GLFW/glfw3.h>


class Camera {
public:
    static Camera* instance;
    
    glm::vec3 position;
    glm::vec3 direction;
    
    GLFWwindow *window;
    
    float speed = 1.0f;
    float sensitivity = 0.1f;
    
    float fov = 45.0f;
   
    bool firstMove = true;
    
    Camera(glm::vec3, glm::vec3, GLFWwindow*, float = 300.0f, float = 0.1f);
    
    void handleInput();
    
    glm::mat4 getViewMatrix();
    
    bool isLookingAt(Character *);
    
private:
    double CAMERA_LAST_X = 0;
    double CAMERA_LAST_Y = 0;
    static bool _scrollCallbackSet;
    
    const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    
    GLfloat lastFrame = 0.0f;
    
    float _yaw = -90.0f;
    float _pitch = 0.0f;
    
    int _forwardKey = GLFW_KEY_W;
    int _backwardKey = GLFW_KEY_S;
    int _leftKey = GLFW_KEY_A;
    int _rightKey = GLFW_KEY_D;
    int _upKey = GLFW_KEY_SPACE;
    int _downKey = GLFW_KEY_LEFT_SHIFT;
    
    
    void _handleMouse();
    void _handleKeyboard(GLfloat);
    void _handleScroll(double);
    
    void _calculateDirection(double xoffset, double yoffset);
};