#include "player.hpp"

// Constructor
Player::Player(std::string name, glm::vec3 basePosition, float screenAspectRatio, float height, float radius)
{
    // Basic Assignment
    this->name = name;
    this->basePosition = basePosition;
    this->height = height > 2.0f ? 2.0f : height; // Don't want extremely long players
    this->radius = radius; // Radius will be used for creating a collider later.
    this->ScreenAspectRatio = screenAspectRatio;
    // Init Player status
    this->ActiveCrouch = false;
    this->ActiveSprint = false;
    this->CrouchState = 0; 
    this->movement_speed = MOVEMENT_SPEED;

    // Initialise the camera
    glm::vec3 cameraPosition = glm::vec3(basePosition.x, basePosition.y + height, basePosition.z);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Positive Y axis is upwards direction
    this->camera = new Camera(cameraPosition, cameraUp);
    this->camera->Front = glm::vec3(0.0f, 0.0f, -1.0f);

    // Probably some collider related stuff

    // Player should handle their own model, view, projection matrices
    this->model = glm::mat4(1.0f);
    this->view = this->camera->GetViewMatrix();
    this->projection = glm::perspective(glm::radians(this->camera->Zoom), screenAspectRatio, 0.1f, 100.0f);

    AppTrace::log(TRACE_LEVEL::INFO, "Successfully created player: " + name);
}

void Player::ActivateSprint( bool active )
{   
    // Can't sprint while crouching
    if(active && ActiveCrouch)
    {
        AppTrace::log(TRACE_LEVEL::VERBOSE, "Can't sprint while crouching!");
        return;
    }

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

    // Handle the crouching logic
    
    // Sit Down
    if( ActiveCrouch && ( ypos > CROUCH_FACTOR * height) )
        ypos -= CROUCH_SPEED * deltaTime;
    
        // Get Back up
    if ( !ActiveCrouch && ( ypos < height) )
        ypos += CROUCH_SPEED * deltaTime;  
    

    // We are an FPS Camera, so we need to disallow movement along y-axis
    this->basePosition.y = ypos;

    // Also, update the player cam position
    this->camera->UpdatePosition(glm::vec3(basePosition.x, basePosition.y + 0.4f, basePosition.z));
}

void Player::UpdateCrouchState(int input)
{
    // Input 0 -> Press, 1 -> Release
    if( CrouchState == 0 )
    {
        // Base Case -> Move to State Pressing if Crouch Pressed
        if ( input == 0 ) CrouchState = 1;
    }
    else if( CrouchState == 1 )
    {
        // Crouch Button Pressing -> Toggle Crouch active & move to base State if Crouch Released
        if ( input  == 1 ) 
        {
            AppTrace::log(TRACE_LEVEL::DEBUG, "Active Crouch: " + std::to_string(ActiveCrouch));
            ActiveCrouch = !ActiveCrouch;
            CrouchState = 0;
        }
    }
}

void Player::UpdatePlayerShader(Shader* shader)
{

    // Update the view and projection matrices
    view = this->camera->GetViewMatrix();
    projection = this->projection = glm::perspective(glm::radians(this->camera->Zoom), this->ScreenAspectRatio, 0.1f, 100.0f);
    
    // Start using this shader
    shader->Use();

    // update the shader with these values
    shader->SetMatrix4("model", this->model);
    shader->SetMatrix4("view", this->view);
    shader->SetMatrix4("projection", this->projection);

    // Pass the normal vector for visualisation
    shader->SetVector3f("cameraFront", this->camera->Front);
}
