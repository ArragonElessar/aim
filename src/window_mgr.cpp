#include "window_mgr.hpp"

GLFWwindow* initWindow(int width, int height, const char* windowTitle)
{
    // 1. Initialize window and provide hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Select core profile
    
    // 2. Create a window
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    if (window == NULL)
    {
        std::string err = "FAILED TO CREATE WINDOW " + std::string(windowTitle); 
        throw std::runtime_error(err);
    }
    glfwMakeContextCurrent(window);

    // 3. Load OpenGl function pointers using GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::string err = "FAILED TO INITIALIZE GLAD FOR " + std::string(windowTitle);
        throw std::runtime_error(err);
    }
    
    // 4. Viewport settings
    glViewport(0, 0, width, height);
    // Set the resize event callback function
    glfwSetFramebufferSizeCallback(window, 
        [](GLFWwindow* window, int newWidth, int newHeight)
        {
            glViewport(0, 0, newWidth, newHeight); // Adjust viewport to window size.
        }
    );
    // 5. Return
    return window;

}
