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

    // Read the whole file at once
    std::string buffer;
    input.seekg(0, std::ios::end);
    buffer.resize(input.tellg());
    input.seekg(0);
    input.read(buffer.data(), buffer.size());
    input.close();

    unsigned int numVertices = 0;

    std::vector<unsigned> lineStartIdxs = {0};
    size_t lineBreakIdx = buffer.find('\n');
    while (lineBreakIdx != std::string::npos)
    {
        buffer[lineBreakIdx] = '\0';
    	++lineBreakIdx;
        lineStartIdxs.push_back(lineBreakIdx);
        lineBreakIdx = buffer.find('\n', lineBreakIdx);
    }

    for (auto & idx: lineStartIdxs)
    {
        std::string_view line{buffer.c_str() + idx};

        if (line.substr(0,2) == "v ")
        {
            float x, y, z;
            if (sscanf_s(line.data(), "v %f %f %f", &x, &y, &z) != 3)
            {
                throw std::runtime_error("Missing vertex coordinate in OBJ file");
            }

            ++numVertices;
            vertices.emplace_back(glm::vec3(x, y, z));
        }
    }

    for (auto& idx : lineStartIdxs)
    {
        std::string_view line{ buffer.c_str() + idx };

        if (line.substr(0,2)=="f ")
        {
            int v1, v2, v3, v4;
            bool quad = false;

            int matched = sscanf_s(line.data(), "f %d%*[^ ] %d%*[^ ] %d%*[^ ] %d\n", &v1, &v2, &v3, &v4);
            if (matched == 4) quad = true;
            else if (matched != 3)
            {
                int matched = sscanf_s(line.data(), "f %d %d %d %d\n", &v1, &v2, &v3, &v4);
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
}
