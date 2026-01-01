#include "physics_engine.hpp"


void PhysicsEngine::AddStaticMesh(Mesh mesh)
{
    // Need to extract triangles from the mesh using vertices, indices
    AppTrace::log(TRACE_LEVEL::VERBOSE, "Adding Static Mesh: " + mesh.name + ", containing " + std::to_string(mesh.vertices.size()) + " vertices and " + std::to_string(mesh.indices.size()) + " indices");
    
    // The # of indices should be a multiple of 3
    if(mesh.indices.size() % 3 != 0)
    {
        std::string errorMsg = "Improper mesh formation, expected multiple of 3 indices, got: " + std::to_string(mesh.indices.size());
        AppTrace::log(TRACE_LEVEL::ERROR, errorMsg);
        throw std::runtime_error(errorMsg);
    }

    // Add to Physics Triangles.
    for(int i = 0; i < mesh.indices.size(); i += 3)
    {
        glm::vec3 a;
        a.x = mesh.vertices[mesh.indices[i]].Position.x;
        a.y = mesh.vertices[mesh.indices[i]].Position.y;
        a.z = mesh.vertices[mesh.indices[i]].Position.z;

        
        glm::vec3 b;
        b.x = mesh.vertices[mesh.indices[i+1]].Position.x;
        b.y = mesh.vertices[mesh.indices[i+1]].Position.y;
        b.z = mesh.vertices[mesh.indices[i+1]].Position.z;

        
        glm::vec3 c;
        c.x = mesh.vertices[mesh.indices[i+2]].Position.x;
        c.y = mesh.vertices[mesh.indices[i+2]].Position.y;
        c.z = mesh.vertices[mesh.indices[i+2]].Position.z;

        Triangle t(a, b, c);
        staticTriangles.push_back(t);
    }

}

void PhysicsEngine::ShowStaticTriangles()
{
    for(auto t : staticTriangles)
    {
        AppTrace::log(TRACE_LEVEL::VERBOSE, t.toString());
    }

    AppTrace::log(TRACE_LEVEL::DEBUG, "Total Static Triangles: " + std::to_string(staticTriangles.size()));
}

glm::vec3 PhysicsEngine::ClosestPointOnTriangle(const glm::vec3 p, const Triangle tri)
{
    glm::vec3 A = tri.a;
    glm::vec3 B = tri.b;
    glm::vec3 C = tri.c;

    // Edge vectors
    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;

    // Vector from A → point
    glm::vec3 AP = p - A;

    // Projections of AP onto AB and AC
    float projAB_AP = glm::dot(AB, AP);
    float projAC_AP = glm::dot(AC, AP);

    // -------- Vertex A region --------
    if (projAB_AP <= 0.0f && projAC_AP <= 0.0f)
        return A;

    // Vector from B → point
    glm::vec3 BP = p - B;
    float projAB_BP = glm::dot(AB, BP);
    float projAC_BP = glm::dot(AC, BP);

    // -------- Vertex B region --------
    if (projAB_BP >= 0.0f && projAC_BP <= projAB_BP)
        return B;

    // -------- Edge AB region --------
    float edgeAB_region_test =
        projAB_AP * projAC_BP - projAB_BP * projAC_AP;

    if (edgeAB_region_test <= 0.0f &&
        projAB_AP >= 0.0f &&
        projAB_BP <= 0.0f)
    {
        float t = projAB_AP / (projAB_AP - projAB_BP); // barycentric param
        return A + t * AB;
    }

    // Vector from C → point
    glm::vec3 CP = p - C;
    float projAB_CP = glm::dot(AB, CP);
    float projAC_CP = glm::dot(AC, CP);

    // -------- Vertex C region --------
    if (projAC_CP >= 0.0f && projAB_CP <= projAC_CP)
        return C;

    // -------- Edge AC region --------
    float edgeAC_region_test =
        projAB_CP * projAC_AP - projAB_AP * projAC_CP;

    if (edgeAC_region_test <= 0.0f &&
        projAC_AP >= 0.0f &&
        projAC_CP <= 0.0f)
    {
        float t = projAC_AP / (projAC_AP - projAC_CP);
        return A + t * AC;
    }

    // -------- Edge BC region --------
    float edgeBC_region_test =
        projAB_BP * projAC_CP - projAB_CP * projAC_BP;

    if (edgeBC_region_test <= 0.0f &&
        (projAC_BP - projAB_BP) >= 0.0f &&
        (projAB_CP - projAC_CP) >= 0.0f)
    {
        float t =
            (projAC_BP - projAB_BP) /
            ((projAC_BP - projAB_BP) + (projAB_CP - projAC_CP));

        return B + t * (C - B);
    }

    // -------- Interior region --------
    glm::vec3 normal = glm::normalize(glm::cross(AB, AC));
    float signedDist = glm::dot(p - A, normal);

    return p - signedDist * normal; // projection onto triangle plane
}

float PhysicsEngine::CalculateSpherePenetration(const glm::vec3 sphereCenter, const float sphereRadius, const Triangle triangle)
{
    glm::vec3 closestPoint = ClosestPointOnTriangle(sphereCenter, triangle);
    float distance = glm::distance(closestPoint, sphereCenter);
    return distance - sphereRadius;
}


glm::vec3 PhysicsEngine::ResolveMovement(Capsule* capsule, const glm::vec3 desiredMove)
{

    /**
     * Given a capsule and our knowledge of the static triangle colliders, 
     * 1. Find the predicted coordinates of our capsule
     * 2. Iterate through all the static meshes (inefficient, but starting)
     *  2.1. Check if the capsule collide with the static mesh
     * 3. We need to find the first collision point ( closest one )
     * 4. Flip the component of the desired move along the collision normal
     * 5. Return calculated move
     */

    glm::vec3 move = desiredMove;
    glm::vec3 top =    capsule->tip   - glm::vec3(0, capsule->radius, 0) + desiredMove;
    glm::vec3 bottom = capsule->base  + glm::vec3(0, capsule->radius, 0) + desiredMove;
    
    for (int i = 0; i < staticTriangles.size(); i++)
    {
        const Triangle& tri = staticTriangles[i];


        float topPen    = CalculateSpherePenetration(top,    capsule->radius, tri);
        float bottomPen = CalculateSpherePenetration(bottom, capsule->radius, tri);

        if (topPen <= EPSILON || bottomPen <= EPSILON)
        {
            // --- depenetrate ---
            float pen = std::min(topPen, bottomPen); // most severe
            glm::vec3 closest = ClosestPointOnTriangle((topPen < bottomPen) ? top : bottom, tri);

            glm::vec3 dir = glm::normalize(((topPen < bottomPen) ? top : bottom) - closest);

            capsule->base = capsule->base - dir * (pen + EPSILON);
            capsule->tip  = capsule->tip - dir * (pen + EPSILON);

            // --- slide remaining movement ---
            AppTrace::log(TRACE_LEVEL::VERBOSE, "Radius: " + std::to_string(capsule->radius) + ", Penetration: " + std::to_string(pen));
            move -= glm::dot(move, tri.normal) * tri.normal;
        }
    }

    return move;
}
