#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <fstream>
#include <string>
#include <sstream> // Required for std::stringstream

#include "mesh.hpp"
#include "model.hpp"

using namespace rapidjson;

// Function to read the entire file content into a std::string
std::string readFileContents(const std::string& filename) {
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

int main()
{
    std::cout << "Hello World from JSON Parse" << std::endl;

    auto jsonDoc = readFileContents("models/basic_room.json");

    // now parse the doc
    Document d;
    d.Parse(jsonDoc.c_str());

    std::cout << "JSON Parsed successfully: " << d.IsObject() << std::endl;

    const Value& meshes = d["meshes"];
    std::cout << "Number of meshes: " << meshes.Size() << std::endl;


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
        std::cout << "Mesh Name: " << mesh_name << std::endl;

        auto json_positions = mesh["positions"].GetArray();
        // print out the vertices
        for(int j = 0; j < json_positions.Size(); j++)
        {
            Vertex v;
            
            const Value& vertex = json_positions[j];
            v.Position.x = static_cast<float>(vertex[0].GetDouble());
            v.Position.y = static_cast<float>(vertex[0].GetDouble());
            v.Position.z = static_cast<float>(vertex[0].GetDouble());
            
            std::cout << "Vertex: " << v.Position.x << ", " << v.Position.y << ", " << v.Position.z << std::endl;
            
            vertices.push_back(v);

        }

        // indices
        auto json_indices = mesh["indices"].GetArray();
        std::cout << "Indices: ";
        for(int j = 0; j < json_indices.Size(); j++)
        {
            indices.push_back(json_indices[j].GetInt());
            std::cout << indices[j] << " "; 
        }
        std::cout << std::endl;

        // textures
        auto json_textures = mesh["texture"].GetString();
        textures.push_back(json_textures);
        std::cout << "Texture: " << textures[0] << std::endl;

        // we need normals per vertice
        auto json_normals = mesh["normals"].GetArray();
        for(int j = 0; j < json_normals.Size(); j++)
        {
            const Value& normal = json_normals[j];
            vertices[j].Normal.x = static_cast<float>(normal[0].GetDouble());
            vertices[j].Normal.y = static_cast<float>(normal[0].GetDouble());
            vertices[j].Normal.z = static_cast<float>(normal[0].GetDouble());

        }

        // TexCoords
        auto json_texCoords = mesh["texCoords"].GetArray();
        for(int j = 0; j < json_texCoords.Size(); j++)
        {
            const Value& texCoord = json_texCoords[j];
            vertices[j].TexCoords.x = static_cast<float>(texCoord[0].GetDouble());
            vertices[j].TexCoords.y = static_cast<float>(texCoord[0].GetDouble());
        }

        Mesh m(vertices, indices, textures, mesh_name);
        room.AddMesh(m);

    }



    return 0;

}