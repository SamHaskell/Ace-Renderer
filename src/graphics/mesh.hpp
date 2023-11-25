#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"
#include "graphics/triangle.hpp"

#include <vector>
#include <string>

#define CUBE_VERT_COUNT 8
#define CUBE_FACE_COUNT 12

namespace Ace {
    extern std::vector<Vec3> g_CubeVertices;
    extern std::vector<Face> g_CubeFaces;

    class Mesh {
        public:
            std::vector<Vec3> Vertices;
            std::vector<Face> Faces;
            
            Vec3 Rotation = {0.0, 0.0, 0.0};
            Vec3 Position = {0.0, 0.0, 0.0};
            Vec3 Scale = {1.0, 1.0, 1.0};

            static Mesh* Create(std::vector<Vec3> vertices, std::vector<Face> faces);
            static Mesh* Load(const std::string& path);
            Mesh() = default;
            ~Mesh() = default;

            Mesh(const Mesh&) = delete;
            Mesh& operator=(Mesh const&) = delete;
    };
}