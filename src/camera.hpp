

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW           = -90.0f;
const float PITCH         =  0.0f;
const float SPEED         =  1.0f;
const float SENSITIVITY   =  0.25f;
const float ZOOM          =  45.0f;
const bool  TRUE_FPS      =  false;
const float SPRINT_FACTOR =  2.0f;
const float CROUCH_FACTOR =  0.7f; // Player's height * Factor upon crouching 
const float CROUCH_SPEED  =  0.5f; // Speed in which player can move up / down during crouching


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool TrueFPS;
    int CrouchState;
    float PlayerHeight;

    // constructor with vectors + FPS
    Camera(bool trueFPS, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        PlayerHeight = Position.y;
        ActiveCrouch = false;
        CrouchState = 0; 
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        TrueFPS = trueFPS;
        Front = glm::vec3(0.0f, 0.0f, 1.0f);
        ActiveSprint = false;
        updateCameraVectors();
    }

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        PlayerHeight = Position.y;
        ActiveCrouch = false;
        CrouchState = 0;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        TrueFPS = TRUE_FPS;
        ActiveSprint = false;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        ActiveCrouch = false;
        PlayerHeight = Position.y;
        CrouchState = 0;
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        TrueFPS = TRUE_FPS;
        ActiveSprint = false;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // handles sprinting and crouching actions
    void ActivateSprint( bool active )
    {
        if(ActiveSprint != active ) std::cout << "[DEBUG] Active Sprint: " << active << std::endl;
        ActiveSprint = active; // Keyboard inputs should handle
    }

    void UpdateCrouch ( int input )
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
                std::cout << "[DEBUG] Active Crouch: " << std::to_string(ActiveCrouch) << std::endl;
                ActiveCrouch = !ActiveCrouch;
                CrouchState = 0;
            }
        }

        // std::cout << "[DEBUG] CrouchState, Input: " << CrouchState << " " << input << std::endl; 

    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(glm::vec3 direction, float deltaTime)
    {
        // Handle sprinting -- TODO the terminology here needs to be improved
        float velocity;
        if(ActiveSprint)
        {
            velocity = SPRINT_FACTOR * MovementSpeed * deltaTime;
        }
        else
        {
            velocity = MovementSpeed * deltaTime;
        }

        // To enable FPS, maintain the same y position
        // TODO - we need to allow jumps later
        float ypos = Position.y;
        if( ActiveCrouch && ( ypos > CROUCH_FACTOR * PlayerHeight) )
        {
            ypos -= CROUCH_SPEED * deltaTime;
        } 
        if ( !ActiveCrouch && ( ypos < PlayerHeight) )
        {
            ypos += CROUCH_SPEED * deltaTime;  
        }

        // Update the position
        Position += direction * velocity;
        
        if(TrueFPS) Position.y = ypos; // Basically, we restrict movement on the y axis
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        yoffset *= MouseSensitivity;
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    
    // Internal Variables
    bool ActiveSprint;
    bool ActiveCrouch;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif

