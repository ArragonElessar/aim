#ifndef __PHYSICS_ENGINE_HPP__
#define __PHYSICS_ENGINE_HPP__

#include <vector>
#include <glm/glm.hpp>

#include "mesh.hpp"
#include "physics_colliders.hpp"
#include "apptrace.hpp"

#define EPSILON 0.0001f

class PhysicsEngine
{
    public:
        void AddStaticMesh(Mesh mesh); // Add a mesh to our system.
        void ShowStaticTriangles();
        glm::vec3 ResolveMovement(Capsule* capsule, const glm::vec3 desiredMove);

    private:
        std::vector<Triangle> staticTriangles;
        glm::vec3 ClosestPointOnTriangle(const glm::vec3 point, const Triangle triangle);
        inline float CalculateSpherePenetration(const glm::vec3 sphereCenter, const float sphereRadius, const Triangle triangle);
};

#endif