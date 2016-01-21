#include <main.hpp>
#include <GLFW/glfw3.h>

class ThirdPersonCamera {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    
    GLFWwindow *window;
    
    float sensitity;
    bool firstMove = true;
    
    ThirdPersonCamera(glm::vec3 p, GLFWwindow *w, float s = 0.1f);
    
    void handleInput(glm::vec3);
    glm::mat4 getViewMatrix(glm::vec3);
    
private:
    double _lastXpos;
    double _lastYpos;
};