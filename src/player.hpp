#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <string>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "shader.hpp"
#include "physics_engine.hpp"
#include "physics_colliders.hpp"

// Some constants related to the player
const float MOVEMENT_SPEED = 1.50f; // m/s
const float SPRINT_FACTOR  = 2.50f; // Sprinting is X times faster than walking
const float CROUCH_FACTOR =  0.50f; // Player's height * Factor upon crouching 
const float CROUCH_SPEED  =  1.50f; // Speed in which player can move up / down during crouching


class Player
{
    public:
        std::string name; // Player Name
        float movement_speed;
        Camera *camera; // This camera is fixed at the player's base position.y + height 
        
        Player(std::string name, glm::vec3 basePosition, float screenAspectRatio=1.33f, float height=1.5f, float radius=0.2f);
        
        // Movement Related functions
        void ActivateSprint( bool active );
        void ProcessKeyboard(glm::vec3 direction, float deltaTime, PhysicsEngine* physicsEngine); // Player processes movement
        void UpdateCrouchState( int input ); // Function to handle button hold logic ( Crouching )

        // Update Shader that draws the player
        void UpdatePlayerShader(Shader* shader);

    private:
        bool ActiveSprint; // Flag for player sprint -- TODO ( idea, later convert this to a 8 bit number to hold player status)
        bool ActiveCrouch;
        int CrouchState; // Variable to check button held down to enable crouching
        float ScreenAspectRatio;
        Capsule* capsule;
        glm::mat4 model, view, projection; // Now player handles these, used for 3D drawing
};

#endif