// Local Headers
#include <main.hpp>

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION

#include <shader.hpp>
#include <camera.hpp>
#include <pipeline.hpp>
#include <polygon.hpp>

#include <prepoly/cube.hpp>
#include <prepoly/pyramid.hpp>

float generateRandomNumber() {
    return static_cast<float>(rand() / (static_cast<float>(RAND_MAX)));
}

int main(void) {
    float mWidth = 1080;
    float mHeight = 600;

    // Load GLFW and Create a Window
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "Polygons", nullptr, nullptr);
    
    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
		getchar();
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    Shader subjectShader;
    
    subjectShader
        .attach("main.vert")
        .attach("main.frag")
        .link();
    
    
    Shader lightShader;
    lightShader
        .attach("main.vert")
        .attach("light.frag")
        .link();
    
	Pyramid subject(glm::vec3(0.0f));
	subject.init(&subjectShader);
	subject.scale(5.0f);
	subject.addTexture("container.jpg", false);
    
    Pyramid light(glm::vec3(10.0f, 10.0f, 0), glm::vec4(1.0f));
    
    light.init(&lightShader);
    light.rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    
    float lasttime = glfwGetTime();
    int frames = 0;
    
    Camera camera(glm::vec3(0, 5.0f, 20.0f), glm::vec3(0, 0, -1.0f), mWindow);
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        frames++;
        float currentTime = glfwGetTime();
        if(currentTime - lasttime >= 1.0) {
            fprintf(stderr, "framerate: %f\r", (float)(frames/(currentTime - lasttime)));
            frames = 0;
            lasttime += 1.0f;
        }
        
        glfwPollEvents();
        
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
        
        camera.handleInput();
        
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), mWidth/mHeight, 0.1f, 150.0f);
        
        float angle = sin(glfwGetTime()/90.0f)*90.0f;
        
        light.orbit(glm::degrees(angle), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0));
        light.draw(view, projection);
        
            subject.shader->activate()
                .bind("lightColor", light.color)
                .bind("lightPos", light.getPosition())
                .bind("viewPos", camera.position);
            
            subject.draw(view, projection);
        
        Pipeline::unbind();
        
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
    }
    
    Pipeline::destroy();
    
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    return EXIT_SUCCESS;
}
