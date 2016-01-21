#include <camera.hpp>
Camera* Camera::instance = nullptr;

Camera::Camera(glm::vec3 p, glm::vec3 d, GLFWwindow *w, float s, float sens) :
    position(p),
    direction(d),
    window(w),
    speed(s),
    sensitivity(sens)
{
    Camera::instance = this;
    glfwSetScrollCallback(window, [](GLFWwindow *, double, double y)->void{
        Camera::instance->_handleScroll(y);
    });
}

void Camera::handleInput() {
    GLfloat currentFrame = glfwGetTime();
    
    _handleMouse();
    _handleKeyboard(currentFrame - lastFrame);
    
    lastFrame = currentFrame;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + direction, CAMERA_UP);
}

bool Camera::isLookingAt(Character *character) {
    Ellipsoid el = character->getEllipsoid();
    glm::vec3 v = position - el.center;
    float q = glm::dot(direction, el.scale*direction);
    float r = glm::dot(v, el.scale*direction);
    float u = glm::dot(direction, el.scale*v);
    float w = glm::dot(v, el.scale*v);
    
    float s = (r+u)*(r+u) - 4*q*(w - 1);
    
    if(s < 0)
        return false;
    
    float t1 = (-2*r - 2*u + sqrtf(s))/(2*q);
    float t2 = (-2*r - 2*u + sqrtf(s))/(2*q);
    
    if(t1 > 0 && t2 > 0)
        return true;
    
    return false;
}

void Camera::_handleMouse() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if(firstMove) {
        CAMERA_LAST_X = xpos;
        CAMERA_LAST_Y = ypos;
        firstMove = false;
    }
    
    _calculateDirection(xpos - CAMERA_LAST_X, CAMERA_LAST_Y - ypos);
    CAMERA_LAST_X = xpos;
    CAMERA_LAST_Y = ypos;
}

void Camera::_handleKeyboard(GLfloat deltaTime) {
    float frameSpeed = speed*deltaTime;
    glm::vec3 right = glm::normalize(glm::cross(direction, CAMERA_UP));
    glm::vec3 up = glm::normalize(glm::cross(right, direction));
    
    if(glfwGetKey(window, _forwardKey) == GLFW_PRESS)
        position += direction*frameSpeed;
    
    if(glfwGetKey(window, _backwardKey) == GLFW_PRESS)
        position -= direction*frameSpeed;
    
    if(glfwGetKey(window, _rightKey) == GLFW_PRESS)
        position += right*frameSpeed;
    
    if(glfwGetKey(window, _leftKey) == GLFW_PRESS)
        position -= right*frameSpeed;
    
    if(glfwGetKey(window, _upKey) == GLFW_PRESS)
        position += up*frameSpeed;
    
    if(glfwGetKey(window, _downKey) == GLFW_PRESS)
        position -= up*frameSpeed;
}

void Camera::_calculateDirection(double xoffset, double yoffset) {
    _yaw += xoffset*sensitivity;
    _pitch += yoffset*sensitivity;
    
    _pitch = fmax(fmin(_pitch, 89.0f), -89.0f);
    
    direction.x = cos(glm::radians(_yaw))*cos(glm::radians(_pitch));
    direction.y = sin(glm::radians(_pitch));
    direction.z = sin(glm::radians(_yaw))*cos(glm::radians(_pitch));
    
    direction = glm::normalize(direction);
}

void Camera::_handleScroll(double y) {
    if(fov >= 1.0f && fov <= 45.0f) fov -= y;
    
    fov = fmax(fmin(fov, 45.0f), 1.0f);
}