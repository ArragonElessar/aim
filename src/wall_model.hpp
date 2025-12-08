#include "object_model.hpp"

class WallModel : public ObjectModel
{   
    private:
        float height, width;
        glm::vec3 center_pos; // Rectangular center of the wall
        glm::vec3 normal;

    protected:
        void generateGeometry();
        void init();

    public:
        WallModel(std::string shaderName, std::string textureName, bool useEBO, bool useTexture, float width, float height, glm::vec3 center_pos, glm::vec3 normal);
        void draw(glm::mat4 projection, glm::mat4 view);

};