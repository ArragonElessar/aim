#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "resource_mgr.hpp"
#include "apptrace.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
    public:
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::string> textures; // This is a vector, but mostly we use only one.

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::string> textures, std::string name = "default");
        void Draw(std::string shaderName);

    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
        void setupTextures();

};
#endif