#include "modelreader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


void readObjFile(std::string filePath, std::vector<TriFace> &faces)
{
    std::vector<glm::vec3> vertices;

    std::ifstream input (filePath);
    if (!input) {
        std::cout << "Couldn't read the file" << std::endl;
        return;
    }

    std::string line;
    unsigned int numVertices = 0;
    while (std::getline(input, line))
    {

        if (line.substr(0,2) == "v ")
        {
            char v;
            float x, y, z;
            if (sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z) != 3)
            {
                throw std::runtime_error("Missing vertex coordinate in OBJ file");
            }

            ++numVertices;
            vertices.emplace_back(glm::vec3(x, y, z));
        }
    }

    // Go back to the start of the file
    input.clear();
    input.seekg(0);

    while (std::getline(input, line))
    {

        if (line.substr(0,2)=="f ")
        {
            int v1, v2, v3, v4;
            bool quad = false;

            int matched = sscanf_s(line.c_str(), "f %d%*[^ ] %d%*[^ ] %d%*[^ ] %d\n", &v1, &v2, &v3, &v4);
            if (matched == 4) quad = true;
            else if (matched != 3)
            {
                int matched = sscanf_s(line.c_str(), "f %d %d %d %d\n", &v1, &v2, &v3, &v4);
                if (matched == 4) quad = true;
                else if (matched != 3) throw std::runtime_error("Missing triangle vertex in OBJ file");
            }

            v1 = (v1 - 1) % numVertices;
            v2 = (v2 - 1) % numVertices;
            v3 = (v3 - 1) % numVertices;

            if (quad)
            {
                v4 = (v4 - 1) % numVertices;

                // If the face is a quad we have to add the two triangles that form it
                faces.emplace_back(TriFace(vertices[v1], vertices[v2], vertices[v3]));
                faces.emplace_back(TriFace(vertices[v3], vertices[v4], vertices[v1]));
            }
            else  // Triangle Face
            {
                faces.emplace_back(TriFace(vertices[v1], vertices[v2], vertices[v3]));
            }
            
        }

    }

    input.close();
}
