#include <iostream>
#include <exception>
#include <vector>
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

#include "level_builder.hpp"

#define SCREEN_WIDTH  1366
#define SCREEN_HEIGHT 768
#define SCREEN_TITLE  "AIM"


Camera *camera;
// Time calculation for the game
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
float currFrameTime = 0.0f;

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    // Abstracting the directions from the keys so that sprinting is balanced
    glm::vec3 direction(0.0f); 

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        direction += camera->Front; // Forward
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        direction -= camera->Front; // Backward
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        direction -= camera->Right; // Left
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        direction += camera->Right; // Right
    }

    // handle sprinting
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
    {
        camera->ActivateSprint( true );
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE )
    {
        camera->ActivateSprint( false );
    }

    // Pass the direction to the camera
    camera->ProcessKeyboard(direction, deltaTime);
}

// Handle the mouse movements
bool firstPos = true;
float lastX, lastY;
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstPos)
    {
        lastX = xpos;
        lastY = xpos;
        firstPos = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}


int32_t main()
{
    // CRUCIAL to enable Logging
    AppTrace::log_depth = TRACE_LEVEL::DEBUG;

    AppTrace::log(TRACE_LEVEL::INFO, "Hello World from Aim Game");

    GLFWwindow* window;
    try{
        window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    }
    catch ( std::exception e )
    {
        std::cout << "EXCEPTION occured while trying to init screen: " << e.what() << std::endl;
        return 0;
    }

    AppTrace::log(TRACE_LEVEL::DEBUG, "Screen Created Successfully");


    // To use the view & projection, we need the camera
    camera = new Camera(true, glm::vec3(0.0f, 0.75f, 1.5f), glm::vec3(0.0f, 1.0f, 0.0f));
    camera->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::mat4 view = camera->GetViewMatrix();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    // Input handlers
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capture cursor input & hide it
    glfwSetCursorPosCallback(window, mouse_movement_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Level builder shit
    Model plane("plane");
    LevelBuilder::ParseJsonToModel("models/basic_room.json", &plane);
    ResourceManager::LoadShader("shaders/model_shader.vs", "shaders/model_shader.fs", nullptr, "model_shader");


    // Enabling depth test
    glEnable(GL_DEPTH_TEST);
    // Main Rendering loop
    while(!glfwWindowShouldClose(window))
    {
        // Update the time
        currFrameTime = glfwGetTime();
        deltaTime = currFrameTime - lastFrameTime;
        lastFrameTime = currFrameTime;

        processInput(window);

        // Screen Background color
        glClearColor(0.5f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the camera 
        view = camera->GetViewMatrix();
        projection = glm::perspective(glm::radians(camera->Zoom), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

        // Set uniform values for the shader
        auto model_shader = ResourceManager::GetShader("model_shader");
        model_shader.Use();
        model_shader.SetMatrix4("model", glm::mat4(1.0f));
        model_shader.SetMatrix4("view", view);
        model_shader.SetMatrix4("projection", projection);

        // Pass the normal vector for visualisation
        model_shader.SetVector3f("cameraFront", camera->Front);

        // Draw the model
        plane.Draw("model_shader");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}