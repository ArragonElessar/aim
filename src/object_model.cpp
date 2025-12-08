#include "object_model.hpp"

ObjectModel::ObjectModel(std::string shaderName, std::string textureName, bool useEBO, bool useTexture)
{
    this->useEBO = useEBO;
    this->shaderName = shaderName;
    this->textureName = textureName;
    this->useTexture = useTexture;

    if( useTexture && textureName == "" ) throw std::runtime_error("NULL Texture not allowed if you want to use texture");

    // Other validations if needed
}

void ObjectModel::draw(glm::mat4 projection, glm::mat4 view){}

void ObjectModel::init(){}

ObjectModel::~ObjectModel(){}