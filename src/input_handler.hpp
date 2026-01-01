#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__

#include <GLFW/glfw3.h>

#include "player.hpp"
#include "apptrace.hpp"
#include "physics_engine.hpp"

class InputHandler
{
    public:
        InputHandler(Player* player);
        void ProcessKeyboardInput(GLFWwindow* window, float deltatime, PhysicsEngine* physicsEngine);
        void MouseMovementCallback(double xpos, double ypos);
        void MouseScrollCallback(double offset);

    private:
        Player* player; // We will pass the movement data to this player
        bool firstMouseMovement;
        float lastX, lastY;
};

InputHandler::InputHandler(Player* player)
{
    // Store the pointer to our player
    this->player = player;
    this->firstMouseMovement = true;
    AppTrace::log(TRACE_LEVEL::INFO, "Initialized the input handler for player: " + this->player->name);
}

void InputHandler::ProcessKeyboardInput(GLFWwindow* window, float deltaTime, PhysicsEngine* physicsEngine)
{
    // Handle all keyboard movements, sprint, crouch

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    // Abstracting the directions from the keys so that sprinting is balanced
    glm::vec3 direction(0.0f); 

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction += player->camera->Front; // Forward
    
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction -= player->camera->Front; // Backward
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction -= player->camera->Right; // Left
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction += player->camera->Right; // Right
    
    // handle sprinting
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
        player->ActivateSprint( true );
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE )
        player->ActivateSprint( false );
    

    // handle crouching 
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS )
        player->UpdateCrouchState( 0 );

    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE )
        player->UpdateCrouchState( 1 );

    // Pass the direction to the player, time
    player->ProcessKeyboard(direction, deltaTime, physicsEngine);
}

void InputHandler::MouseMovementCallback(double xpos, double ypos)
{

    if (!player || !player->camera)
    {
        AppTrace::log(TRACE_LEVEL::ERROR, "Null player or camera in MouseMovementCallback!");
        return;
    }

    // Set the parameters if this is the first mourse move
    if(firstMouseMovement)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouseMovement = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    player->camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void InputHandler::MouseScrollCallback(double offset)
{
    if (!player || !player->camera)
    {
        AppTrace::log(TRACE_LEVEL::ERROR, "Null player or camera in MouseMovementCallback!");
        return;
    }
    player->camera->ProcessMouseScroll(offset);
}


#endif