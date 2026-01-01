#ifndef __PHYSICS_COLLIDERS_HPP__
#define __PHYSICS_COLLIDERS_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class Triangle
{
    public:
        glm::vec3 a, b, c; // Three vertices make up a triangle -> A will be treated specially.
        glm::vec3 normal; // Normal direction to this triagnle face. 
        Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
        {
            this->a = a;
            this->b = b;
            this->c = c;

            // Normal is along the cross product of edges AB, AC
            glm::vec3 edgeAB = b - a;
            glm::vec3 edgeAC = c - a;
            this->normal = glm::normalize(glm::cross(edgeAB, edgeAC)); // Normalized
        }
        std::string toString()
        {
            std::string ret = "";
            ret += "\n\tA: " + glm::to_string(a) + "\n";
            ret += "\tB: " + glm::to_string(b) + "\n";
            ret += "\tC: " + glm::to_string(c) + "\n";
            ret += "\tN: " + glm::to_string(normal) + "\n";

            return ret;
        }
};

class Capsule
{
    // Capsule is two spheres - top, bottom and center cylinder
    // Center points for both are relative to base and tip
    public:
        glm::vec3 base, tip; // Represent top and bottom positions of this capsule
        float radius, height;
        Capsule(glm::vec3 base, float height, float radius)
        {
            this->base = base;
            this->height = height;
            this->tip = base + glm::vec3(0.0f, height, 0.0f);
            this->radius = radius;
        }
};
#endif



