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
#include "wall_model.hpp"
#include "camera.hpp"

#define SCREEN_WIDTH  2560
#define SCREEN_HEIGHT 1440
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

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime); // Move towards where the camera is facing
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime); // Move away from where camera is facing
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // We need to find the right vector -> cross between up and camera facing 
        camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);  
    }
}

// Handle the mouse movements
bool firstPos = true;
float lastX, lastY;
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos)
{
    // std::cout << "[DEBUG] X, Y: (" << xpos << ", " << ypos << "), Camera Pointing at: " << camera->Front.x << ", " << camera->Front.y << ", " << camera->Front.z << ", Position: " << camera->Position.x << ", " << camera->Position.y << ", " << camera->Position.z << std::endl;
    
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
    std::cout << "[DEBUG] Hello World!" << std::endl;

    GLFWwindow* window;
    try{
        window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    }
    catch ( std::exception e )
    {
        std::cout << "EXCEPTION occured while trying to init screen: " << e.what() << std::endl;
        return 0;
    }

    std::cout << "[DEBUG] Successfully Created Screen" << std::endl;


    // To use the view & projection, we need the camera
    camera = new Camera(true, glm::vec3(0.0f, 0.35f, 1.5f), glm::vec3(0.0f, 1.0f, 0.0f));
    camera->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::mat4 view = camera->GetViewMatrix();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    // Input handlers
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capture cursor input & hide it
    glfwSetCursorPosCallback(window, mouse_movement_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Wall definitions
    float rectWidth = 1.0f, rectHeight = 1.0f;
    std::vector<std::vector<glm::vec3>> wallDefinitions = {

        // Left side walls
        { glm::vec3(-1.0f, 0.5f,  1.5f), glm::vec3(-1.0f, 0.0f, 0.0f)},
        { glm::vec3(-1.0f, 0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f)},
        { glm::vec3(-1.0f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f)},
        { glm::vec3(-1.0f, 0.5f, -1.5f), glm::vec3(-1.0f, 0.0f, 0.0f)},

        // Right Side Walls
        { glm::vec3(1.0f, 0.5f,  1.5f), glm::vec3(1.0f, 0.0f, 0.0f)},
        { glm::vec3(1.0f, 0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f)},
        { glm::vec3(1.0f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f)},
        { glm::vec3(1.0f, 0.5f, -1.5f), glm::vec3(1.0f, 0.0f, 0.0f)},

        // Bottom Walls
        { glm::vec3(-0.5f, 0.0f,  1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(-0.5f, 0.0f, -1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},

        { glm::vec3(0.5f, 0.0f,  1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(0.5f, 0.0f, -1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},

        // Top Walls
        { glm::vec3(-0.5f, 1.0f,  1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(-0.5f, 1.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(-0.5f, 1.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(-0.5f, 1.0f, -1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},

        { glm::vec3(0.5f, 1.0f,  1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(0.5f, 1.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(0.5f, 1.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},
        { glm::vec3(0.5f, 1.0f, -1.5f), glm::vec3(0.0f, 1.0f, 0.0f)},

    };

    std::vector<std::unique_ptr<WallModel>> walls;
    for( auto wallDef: wallDefinitions)
    {
        walls.push_back(std::make_unique<WallModel>(
            "shaders/basic", "assets/brick-wall.jpg",
            true, true,
            rectWidth, rectHeight,
            wallDef[0], wallDef[1]
        ));
    }

    // push the grey walls
    std::vector<std::vector<glm::vec3>> greyWallDefinitions = {
        {glm::vec3( 0.5f, 0.5f, -2.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f, 0.5f, -2.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3( 0.5f, 0.5f,  2.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f, 0.5f,  2.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
    };
    for ( auto greyWallDef : greyWallDefinitions)
    {
        walls.push_back(std::make_unique<WallModel>(
            "shaders/basic", "assets/gray-wall.jpg",
            true, true,
            rectWidth, rectHeight,
            greyWallDef[0], greyWallDef[1]
        ));
    }
    


    // Enabling depth testr
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

        // Draw the walls
        for(auto& wall: walls)
        {
            wall->draw(projection, view);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}