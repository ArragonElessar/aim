#ifndef __GAME_LEVEL_BUILDER_HPP__
#define __GAME_LEVEL_BUILDER_HPP__

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <fstream>
#include <string>
#include <sstream> // Required for std::stringstream

#include "mesh.hpp"
#include "model.hpp"
#include "apptrace.hpp"

using namespace rapidjson;

class LevelBuilder
{
    public:
        static void ParseJsonToModel(std::string jsonFileName, Model* model);
    private:
        static std::string readFileContents(const std::string& filename); 

};

std::string LevelBuilder::readFileContents(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return ""; // Return an empty string on failure
    }

    // Read the file content into a string using a stringstream
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close(); // Close the file stream

    return buffer.str();
}

void LevelBuilder::ParseJsonToModel(std::string jsonFileName, Model* model)
{
    // We expect a standard JSON Format
    auto jsonDoc = readFileContents(jsonFileName);

    // now parse the doc
    Document d;
    d.Parse(jsonDoc.c_str());

    AppTrace::log(TRACE_LEVEL::DEBUG, "JSON Parsed successfully: " + std::to_string(d.IsObject()));

    const Value& meshes = d["meshes"];
    AppTrace::log(TRACE_LEVEL::DEBUG, "Number of meshes: " + std::to_string(meshes.Size()));


    Model room("room");

    for(int i = 0; i < meshes.Size(); i++)
    {
        // Start parsing the mesh
        std::string mesh_name;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::string> textures;

        // parse the ith mesh
        const Value& mesh = meshes[i];
        mesh_name = mesh["name"].GetString();
        AppTrace::log(TRACE_LEVEL::DEBUG, "Mesh Name: " + mesh_name);

        auto json_positions = mesh["positions"].GetArray();
        // print out the vertices
        for(int j = 0; j < json_positions.Size(); j++)
        {
            Vertex v;
            
            const Value& vertex = json_positions[j];
            v.Position.x = static_cast<float>(vertex[0].GetDouble());
            v.Position.y = static_cast<float>(vertex[1].GetDouble());
            v.Position.z = static_cast<float>(vertex[2].GetDouble());
            
            AppTrace::log(TRACE_LEVEL::DEBUG, "Vertex: " + std::to_string(v.Position.x) + ", " + std::to_string(v.Position.y) + ", " + std::to_string(v.Position.z));
            
            vertices.push_back(v);

        }

        // indices
        auto json_indices = mesh["indices"].GetArray();
        std::stringstream ss;
        ss << "Indices: ";
        for(int j = 0; j < json_indices.Size(); j++)
        {
            indices.push_back(json_indices[j].GetInt());
            ss << indices[j] << " ";
        }
        AppTrace::log(TRACE_LEVEL::DEBUG, ss.str());

        // textures
        auto json_textures = mesh["texture"].GetString();
        textures.push_back(json_textures);
        AppTrace::log(TRACE_LEVEL::DEBUG, "Texture: " + textures[0]);

        // we need normals per vertice
        auto json_normals = mesh["normals"].GetArray();
        for(int j = 0; j < json_normals.Size(); j++)
        {
            const Value& normal = json_normals[j];
            vertices[j].Normal.x = static_cast<float>(normal[0].GetDouble());
            vertices[j].Normal.y = static_cast<float>(normal[1].GetDouble());
            vertices[j].Normal.z = static_cast<float>(normal[2].GetDouble());

        }

        // TexCoords
        auto json_texCoords = mesh["texCoords"].GetArray();
        for(int j = 0; j < json_texCoords.Size(); j++)
        {
            const Value& texCoord = json_texCoords[j];
            vertices[j].TexCoords.x = static_cast<float>(texCoord[0].GetDouble());
            vertices[j].TexCoords.y = static_cast<float>(texCoord[1].GetDouble());

            AppTrace::log(TRACE_LEVEL::DEBUG, "TexCoords (x, y): " + std::to_string(vertices[j].TexCoords.x) + ", " + std::to_string(vertices[j].TexCoords.y));
        }

        Mesh m(vertices, indices, textures, mesh_name);
        model->AddMesh(m);
    }

}
#endif