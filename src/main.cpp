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

#define SCREEN_WIDTH  1366
#define SCREEN_HEIGHT 768
#define SCREEN_TITLE  "AIM"

// Function signatures
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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

    // Create the walls that we need.
    WallModel wall_a("shaders/basic", "assets/brick-wall.jpg", true, true, 0.5f, 0.5f, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.5f));
    WallModel wall_b("shaders/basic", "assets/brick-wall.jpg", true, true, 0.5f, 0.5f, glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(-0.5f, 0.0f, 0.5f));
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, -0.25f, -2.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    // Enabling depth testr
    glEnable(GL_DEPTH_TEST);
    // Main Rendering loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Screen Background color
        glClearColor(0.5f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // for(auto &obj: objects) obj->draw(projection, view);
        wall_a.draw(projection, view);
        wall_b.draw(projection, view);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}