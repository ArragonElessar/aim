#include <iostream>
#include <exception>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window_mgr.hpp"
#include "resource_mgr.hpp"

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

    // Get the basic shaders
    ResourceManager::LoadShader("shaders/basic.vs", "shaders/basic.fs", NULL, "basic");

    // Draw a basic square
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top Left
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom Right
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f  // Top Right
    };

    std::vector<unsigned int> indices = { 
        0, 1, 2, 
        1, 2, 3 
    }; // Order in which vertices should be drawn

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    const size_t stride = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Onto Textures
    ResourceManager::LoadTexture("assets/brick-wall.jpg", false, "wallTex");
    
    // Main Rendering loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Screen Background color
        glClearColor(0.5f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start using the shader
        ResourceManager::GetShader("basic").Use();

        // Apply the texture
        ResourceManager::GetShader("basic").SetInteger("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        ResourceManager::GetTexture("wallTex").Bind();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}