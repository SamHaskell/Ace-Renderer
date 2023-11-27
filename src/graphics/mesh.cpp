#include "graphics/mesh.hpp"

#include <stdio.h>

namespace Ace {
    Mesh* Mesh::Create(std::vector<Vec3> vertices, std::vector<Vec2> texCoords, std::vector<Face> faces) {
        Mesh* mesh = new Mesh();
        mesh->Vertices = vertices;
        mesh->TexCoords = texCoords;
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

        i8 line[1024];
        while(fgets(line, 1024, file)) {
            if (strncmp(line, "v ", 2) == 0) {
                // We have a vertex!
                Vec3 vert;
                sscanf(line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
                mesh->Vertices.push_back(vert);
            }

            if (strncmp(line, "vt ", 3) == 0) {
                // We have a tex-coord!
                Vec2 uv;
                sscanf(line, "vt %f %f", &uv.x, &uv.y);
                uv.y = 1.0f - uv.y;
                mesh->TexCoords.push_back(uv);
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
                    .a = indices[0] - 1,
                    .b = indices[3] - 1,
                    .c = indices[6] - 1,

                    .aUV = indices[1] - 1,
                    .bUV = indices[4] - 1,
                    .cUV = indices[7] - 1
                };

                mesh->Faces.push_back(vertFace);
            }
        }

        fclose(file);

        return mesh;
    }
};