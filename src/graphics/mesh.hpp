#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"
#include "graphics/triangles.hpp"

#include <vector>
#include <string>

namespace Ace {
    class Mesh {
        public:
            std::vector<Vec3> Vertices;
            std::vector<Vec2> TexCoords;
            std::vector<Face> Faces;
            
            Vec3 Rotation = {0.0, 0.0, 0.0};
            Vec3 Position = {0.0, 0.0, 0.0};
            Vec3 Scale = {1.0, 1.0, 1.0};

            static Mesh* Create(std::vector<Vec3> vertices, std::vector<Vec2> texCoords, std::vector<Face> faces);
            static Mesh* Load(const std::string& path);
            Mesh() = default;
            ~Mesh() = default;

            Mesh(const Mesh&) = delete;
            Mesh& operator=(Mesh const&) = delete;
    };
}