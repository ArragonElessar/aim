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

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::string> textures, std::string name)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->name = name;

    setupMesh();
    setupTextures();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::setupTextures()
{
    for(std::string texture : textures)
    {
        AppTrace::log(TRACE_LEVEL::DEBUG, "Loading Texture: " + texture);
        ResourceManager::LoadTexture(texture.c_str(), false, texture.c_str());
    }
}

void Mesh::Draw(std::string shaderName)
{
    // Here, the assumption is that model, view, projection matrices are set before calling this.

    if(textures.size() > 1) throw std::runtime_error("[ERROR] More than 1 texture not supported yet.");
    // Apply the texture
    ResourceManager::GetShader(shaderName).SetInteger("tex", 0);
    glActiveTexture(GL_TEXTURE0);
    ResourceManager::GetTexture(textures[0]).Bind();

    // We also assume that all object are drawn using elements
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#endif