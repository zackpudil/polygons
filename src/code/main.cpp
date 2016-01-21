// Local Headers
#include <main.hpp>

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <map>

#define STB_IMAGE_IMPLEMENTATION

#include <shader.hpp>

#include <camera.hpp>
#include <thirdPersonCamera.hpp>

#include <model.hpp>
#include <stage.hpp>
#include <character.hpp>

#include <animctlr.hpp>
#include <anim.hpp>

#include <cursor.hpp>

int main(void) {
    float width = 1080;
    float height = 600;

    // Load GLFW and Create a Window
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto window = glfwCreateWindow(width, height, "Polygons", nullptr, nullptr);

    // Check for Valid Context
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
		getchar();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glViewport(0, 0, width*2, height*2);


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Shader subjectShader;
    subjectShader
        .attach("character.vert")
        .attachLibrary("phong.glsl")
        .attach("material.frag")
        .link();

    Shader subjectShadowShader;
    subjectShadowShader
        .attach("shadow_character.vert")
        .attach("shadow.geom")
        .attach("shadow.frag")
        .link();

    Shader stageShader;
    stageShader
        .attach("stage.vert")
        .attachLibrary("phong.glsl")
        .attach("material.frag")
        .link();

    Shader stageShadowShader;
    stageShadowShader
        .attach("shadow_stage.vert")
        .attach("shadow.geom")
        .attach("shadow.frag")
        .link();

    Stage stage("small_stage/small_stage.dae", &stageShader, &stageShadowShader);

    std::vector<Character*> containerGuys;
    Model* containerGuyModel = new Model("container_guy/container_guy.fbx");

    for(uint i = 0; i < 6; i++) {
        Animator* containerGuyAnim = new Animator(AnimationController(containerGuyModel));
        Character* containerGuy = new Character(&subjectShader, &subjectShadowShader,
                                                containerGuyAnim, containerGuyModel,
                                                glm::vec3(10, 0, i*20), 0.4f, 2.0f, 0.02f);

        containerGuy->idleToWalk = [](Animator* a) { return &a->play(1, 0, {10}, {}, true); };
        containerGuy->walk = [](Animator* a) { return &a->loop(1, 10, {56}, {23, 43}, true); };

        containerGuy->walkToIdles = {[](Animator* a) {
            return &a->play(1, 0, {23, 56}, {10, 56}, false);
        }, [](Animator *a) {
            return &a->play(1, 0, {33, 65}, {}, false);
        }};

        containerGuy->idle = [](Animator* a) {
            return &a->play(0, 0, {50}, {}, true)
                .loop(0, 50, {150}, {}, true);

        };

        containerGuys.push_back(containerGuy);
    }

    int activeContainer = -1;

    float lasttime = glfwGetTime();
    int frames = 0;

    Camera camera(glm::vec3(0.0f, 25.0f, 40.0f), glm::vec3(0), window);
    ThirdPersonCamera thirdCamera(glm::vec3(0), window);

    Cursor cursor(width, height, glm::vec3(1));

    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 viewPos;

    float far_plane = 890.0f;

    std::function<void(PointLight)> renderShadows = [&](PointLight p) {
        stageShadowShader.activate();
        stage.drawShadow(p.getTransforms(far_plane), p.position, far_plane);

        subjectShadowShader.activate();
        for(auto g : containerGuys)
            g->drawShadow(p.getTransforms(far_plane), p.position, far_plane);
    };

    std::function<void()> renderScene = [&]() {

        stageShader.activate().bind("viewPos", viewPos).bind("far_plane", far_plane);
        stage.lights.light({ &stageShader });
        stage.draw(projection, view);

        subjectShader.activate().bind("viewPos", viewPos).bind("far_plane", far_plane);
        stage.lights.light({ &subjectShader });

        for(auto g : containerGuys)
            g->draw(projection, view);

        if(activeContainer == -1)
            for(auto g : containerGuys)
                if(camera.isLookingAt(g))
                    g->drawOutline(projection, view);

        cursor.draw();
    };

    bool mouseDown = false;

    for (uint i = 0; i < stage.lights._pointLights.size(); i++)
        Pipeline::createShadowFrame(1024, 1024);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // Rendering Loop
    while (glfwWindowShouldClose(window) == false) {
        frames++;
        float currentTime = glfwGetTime();
        if(currentTime - lasttime >= 1.0) {
            fprintf(stderr, "framerate: %f\r", (float)(frames/(currentTime - lasttime)));
            frames = 0;
            lasttime += 1.0f;
        }

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        projection = glm::perspective(45.0f, width/height, 0.1f, 1500.0f);
        view = activeContainer != -1
            ? thirdCamera.getViewMatrix(containerGuys[activeContainer]->position)
            : camera.getViewMatrix();

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            activeContainer = -1;

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if(activeContainer == -1)
                mouseDown = true;
        } else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && mouseDown) {

            for(uint i = 0; i < containerGuys.size(); i++)
                if(camera.isLookingAt(containerGuys[i]))
                    activeContainer = i;

            mouseDown = false;
        }

        if(activeContainer != -1) {
            thirdCamera.handleInput(containerGuys[activeContainer]->position);
            containerGuys[activeContainer]->handleInput(window);
            camera.firstMove = true;
        } else {
            thirdCamera.firstMove = true;
            camera.handleInput();
        }

        for(int i = 0; i < (int)containerGuys.size(); i++) {

            if(i != activeContainer)
                containerGuys[i]->emulateStop();
        }

        viewPos = activeContainer != -1 ? thirdCamera.position : camera.position;


        glViewport(0, 0, 1024, 1024);

        auto it = Pipeline::shadowCubeMaps.begin();

        for(auto p : stage.lights._pointLights) {
            auto d = *it;
            glBindFramebuffer(GL_FRAMEBUFFER, d.second);
                glClear(GL_DEPTH_BUFFER_BIT);
                renderShadows(p);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            it++;
        }

        glViewport(0, 0, width*2, height*2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        renderScene();

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
