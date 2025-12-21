#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <string>
#include <vector>

#include "mesh.hpp"
#include "apptrace.hpp"

class Model
{
    public:
        Model(std::string name);
        void Draw(std::string shaderName);
        void AddMesh(Mesh mesh);

    private:
        std::vector<Mesh> meshes;
        std::string name;
};

Model::Model(std::string name){
    this->name = name;
}

void Model::Draw(std::string shaderName)
{
    // We expect all meshes to be compatible with this shader
    for( auto& mesh: meshes)
    {
        mesh.Draw(shaderName);
    }
}

void Model::AddMesh(Mesh mesh)
{
    meshes.push_back(mesh);
    AppTrace::log(TRACE_LEVEL::DEBUG, "Added Mesh: " + mesh.name);
}

#endif