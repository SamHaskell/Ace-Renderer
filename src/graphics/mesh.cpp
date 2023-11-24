#include "graphics/mesh.hpp"

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

    }
};