#include "wall_model.hpp"

WallModel::WallModel(std::string shaderName, std::string textureName, bool useEBO, bool useTexture, float width, float height, glm::vec3 center_pos, glm::vec3 normal)
: ObjectModel(shaderName, textureName, useEBO, useTexture)
{
    // Our walls should always use EBOs
    if ( !useEBO ) throw std::runtime_error("Walls should use element buffers");

    // Height and width should be within ( -1.0, 1.0 )
    if ( height > 1.0 || height < -1.0 ) throw std::runtime_error("Wall height expected between (-1.0, 1.0)");
    if ( width > 1.0 || width < -1.0 ) throw std::runtime_error("Wall width expected between (-1.0, 1.0)");

    this->height = height;
    this->width = width;
    this->center_pos = center_pos;
    this->normal = normal;

    // Generate geometry
    this->generateGeometry();

    // init the drawing elements
    this->init();

}

void WallModel::generateGeometry()
{
    this->vertices = {
        // X                 Y               Z     Texture Coords
        -this->width / 2, -this->height / 2, 0.0f, 0.0f, 0.0f, // Bottom Left
        -this->width / 2,  this->height / 2, 0.0f, 0.0f, 1.0f, // Top Left
         this->width / 2, -this->height / 2, 0.0f, 1.0f, 0.0f, // Bottom Right
         this->width / 2,  this->height / 2, 0.0f, 1.0f, 1.0f,  // Top Right        
    };

    this->indices = {
        0, 1, 2, 
        1, 2, 3,
    }; // Order in which vertices should be drawn
}

void WallModel::init()
{
    // Generate the Vertex Array Buffer and the Vertex Buffer Objects
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    if(useEBO) glGenBuffers(1, &this->EBO);
    
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    
    if(useEBO)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);
    }
    
    // This is based on our current implementation of a textured wall
    const size_t stride = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load the shaders and textures
    ResourceManager::LoadShader((this->shaderName + ".vs").c_str(), (this->shaderName + ".fs").c_str(), nullptr, this->shaderName);
    if(useTexture) ResourceManager::LoadTexture(this->textureName.c_str(), false, this->textureName);
    
}

void WallModel::draw(glm::mat4 projection, glm::mat4 view)
{
    // Use our shader
    ResourceManager::GetShader(this->shaderName).Use();

    // Update the model matrix by translating and rotating to our position
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->center_pos); // translated

    /**
     * For rotation to match the normal, angle is calculated by using the dot product
     * And the axis is along the cross product
     */

    auto face = glm::vec3(0.0f, 0.0f, 1.0f);
    auto axis = glm::cross(face, this->normal);
    auto angle = glm::acos( glm::dot(face, this->normal) ); // Here it is sure that the vectors are unit vectors.

    // If we are already along the normal we don't need to rotate
    if( angle != 0.0f ) model = glm::rotate(model, angle, axis);

    // Write the Model, View, projection matrices to the shader
    ResourceManager::GetShader(this->shaderName).SetMatrix4("model", model);
    ResourceManager::GetShader(this->shaderName).SetMatrix4("view", view);
    ResourceManager::GetShader(this->shaderName).SetMatrix4("projection", projection);

    // Use the Textures
    if(useTexture)
    {
        // Apply the texture
        ResourceManager::GetShader(this->shaderName).SetInteger("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        ResourceManager::GetTexture(this->textureName).Bind();
    }

    glBindVertexArray(this->VAO);

    if(useEBO) glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    else{
        throw std::runtime_error("Current Walls should use the EBOs always!");
    } 

}