#include "graphics/mesh.hpp"

#include <stdio.h>

namespace Ace {
    std::vector<Vec3> g_CubeVertices = {
        {-1.0, -1.0, -1.0},
        {-1.0, +1.0, -1.0},
        {+1.0, +1.0, -1.0},
        {+1.0, -1.0, -1.0},
        {+1.0, +1.0, +1.0},
        {+1.0, -1.0, +1.0},
        {-1.0, +1.0, +1.0},
        {-1.0, -1.0, +1.0}
    };

    std::vector<Face> g_CubeFaces = {
        { 0, 1, 2 },
        { 0, 2, 3 },
        { 3, 2, 4 },
        { 3, 4, 5 },
        { 5, 4, 6 },
        { 5, 6, 7 },
        { 7, 6, 1 },
        { 7, 1, 0 },
        { 1, 6, 4 },
        { 1, 4, 2 },
        { 5, 7, 0 },
        { 5, 0, 3 }
    };  

    Mesh* Mesh::Create(std::vector<Vec3> vertices, std::vector<Face> faces) {
        Mesh* mesh = new Mesh();
        mesh->Vertices = vertices;
        mesh->Faces = faces;
        return mesh;
    }
    
    Mesh* Mesh::Load(const std::string& path) {
        Mesh* mesh = new Mesh();

        FILE* file = fopen(path.c_str(), "r");
        if (file == NULL) {
            ACE_ERROR("Failed to load mesh %s from disk", path.c_str());
            return nullptr;
        }

        u8 line[1024];
        while(fgets(line, 1024, file)) {
            if (strncmp(line, "v ", 2) == 0) {
                // We have a vertex!
                Vec3 vert;
                sscanf(line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
                mesh->Vertices.push_back(vert);
            }

            if (strncmp(line, "f ", 2) == 0) {
                // We have a face!
                u32 indices[9];
                sscanf(
                    line, 
                    "f %i/%i/%i %i/%i/%i %i/%i/%i",
                    &indices[0], &indices[1], &indices[2],
                    &indices[3], &indices[4], &indices[5],
                    &indices[6], &indices[7], &indices[8]
                );
                
                Face vertFace = {
                    indices[0] - 1,
                    indices[3] - 1,
                    indices[6] - 1
                };

                mesh->Faces.push_back(vertFace);
            }
        }

        fclose(file);

        return mesh;
    }
};