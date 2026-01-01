#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "window_mgr.hpp"
#include "resource_mgr.hpp"
#include "camera.hpp"
#include "apptrace.hpp"
#include "player.hpp"
#include "level_builder.hpp"
#include "input_handler.hpp"
#include "physics_engine.hpp"

#define SCREEN_WIDTH  1366
#define SCREEN_HEIGHT 768
#define SCREEN_TITLE  "AIM"

int32_t main()
{
    // CRUCIAL to enable Logging
    AppTrace::log_depth = TRACE_LEVEL::VERBOSE;
    AppTrace::log(TRACE_LEVEL::INFO, "Hello World from Aim Game");

    // Create the window
    GLFWwindow* window;
    window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    AppTrace::log(TRACE_LEVEL::DEBUG, "Screen Created Successfully");

    // Create the Player, as well as pass the screen aspect ratio
    Player* player = new Player("pranav", glm::vec3(0.0f, 0.0f, 1.5f), (float)SCREEN_WIDTH / SCREEN_HEIGHT);
    
    // Create the input handler
    InputHandler* inputHandler = new InputHandler(player); 
    glfwSetWindowUserPointer(window, inputHandler); // Pass the input object as context to the window

    // Input handlers
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capture cursor input & hide it
    glfwSetCursorPosCallback(window, 
        [](GLFWwindow* w, double x, double y)
        {
            auto *ih = static_cast<InputHandler*>(glfwGetWindowUserPointer(w));
            ih->MouseMovementCallback(x, y);
        }
    );
    glfwSetScrollCallback(window, 
        [](GLFWwindow* w, double x, double y)
        {
            auto *ih = static_cast<InputHandler*>(glfwGetWindowUserPointer(w));
            ih->MouseScrollCallback(y); // Only need scrolling in y-direction ( mouse scroll wheel )
        }
    );

    // Level builder
    Model room("room_model");
    LevelBuilder::ParseJsonToModel("models/basic_room.json", &room);
    ResourceManager::LoadShader("shaders/model_shader.vs", "shaders/model_shader.fs", nullptr, "model_shader");

    // Physics related stuff
    PhysicsEngine physicsEngine;
    for(auto m : room.meshes)
    {
        AppTrace::log(TRACE_LEVEL::DEBUG, "Adding Mesh (name): " + m.name);
        physicsEngine.AddStaticMesh(m);
    }
    physicsEngine.ShowStaticTriangles();
    

    // Enabling depth test
    glEnable(GL_DEPTH_TEST);

    // Time calculation for the game
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;
    float currFrameTime = 0.0f;

    // Main Rendering loop
    while(!glfwWindowShouldClose(window))
    {
        // Update the time
        currFrameTime = glfwGetTime();
        deltaTime = currFrameTime - lastFrameTime;
        lastFrameTime = currFrameTime;

        inputHandler->ProcessKeyboardInput(window, deltaTime, &physicsEngine);

        // Screen Background color
        glClearColor(0.5f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set uniform values for the shader, now handled by the player
        auto model_shader = ResourceManager::GetShader("model_shader");
        player->UpdatePlayerShader(&model_shader);

        // Draw the model
        room.Draw("model_shader");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}