#include "player.hpp"

// Constructor
Player::Player(std::string name, glm::vec3 basePosition, float height, float radius)
{
    // Basic Assignment
    this->name = name;
    this->basePosition = basePosition;
    this->height = std::max(2.0f, height); // Don't want extremely long players
    this->radius = radius; // Radius will be used for creating a collider later.

    // Init Player status
    this->ActiveCrouch = false;
    this->ActiveSprint = false; 
    this->movement_speed = MOVEMENT_SPEED;

    // Initialise the camera
    glm::vec3 cameraPosition = glm::vec3(basePosition.x, basePosition.y + height, basePosition.z);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Positive Y axis is upwards direction
    this->camera = new Camera(cameraPosition, cameraUp);
    this->camera->Front = glm::vec3(0.0f, 0.0f, -1.0f);

    // Probably some collider related stuff

    AppTrace::log(TRACE_LEVEL::INFO, "Successfully created player: " + name);
}

void Player::ActivateSprint( bool active )
{
    if(ActiveSprint != active ) AppTrace::log(TRACE_LEVEL::DEBUG, "Active Sprint Status: " + std::to_string(active));
    ActiveSprint = active; // Keyboard inputs should handle
}

void Player::ProcessKeyboard(glm::vec3 direction, float deltaTime)
{
    // Calculate the step length basis if sprint is active or not.
    float step_length = ActiveSprint ? SPRINT_FACTOR * movement_speed * deltaTime : movement_speed * deltaTime;

    // TODO - Include crouching here.
    float ypos = this->basePosition.y;

    // Update the movement according to keyboard input
    // TODO - here consult the physics engine
    this->basePosition += direction * step_length;

    // We are an FPS Camera, so we need to disallow movement along y-axis
    this->basePosition.y = ypos;

    // Also, update the player cam position
    this->camera->UpdatePosition(glm::vec3(basePosition.x, basePosition.y + 1.0f, basePosition.z));
}
