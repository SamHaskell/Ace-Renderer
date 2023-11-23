#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"
#include "graphics/triangle.hpp"

#define MESH_VERT_COUNT 8
#define MESH_FACE_COUNT 12

namespace Ace {
    extern Vec3 g_MeshVertices[MESH_VERT_COUNT];
    extern Face g_MeshFaces[MESH_FACE_COUNT];
}