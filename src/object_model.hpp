#ifndef __OBJECT_MODEL_HPP__
#define __OBJECT_MODEL_HPP__

#include <vector>
#include <string>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "resource_mgr.hpp"

class ObjectModel
{

    protected:
        std::string shaderName;
        std::string textureName;
        bool useEBO;
        bool useTexture; // Depends on if we are using a texture as defined in the shader

        unsigned int VAO, VBO, EBO;

        std::vector<float> vertices; // Format should be same as the one used in the shader
        std::vector<unsigned int> indices; // if using EBO

        // Subclasses should implement this
        void generateGeometry();
        void init();

    public:
        ObjectModel(std::string shaderName, std::string textureName, bool useEBO, bool useTexture);
        ~ObjectModel();
        void draw(glm::mat4 projection, glm::mat4 view);


};

#endif