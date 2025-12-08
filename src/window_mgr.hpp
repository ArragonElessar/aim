#ifndef __WINDOW_MGR_HPP__
#define __WINDOW_MGR_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

GLFWwindow* initWindow( int width, int height, const char* windowTitle );

#endif