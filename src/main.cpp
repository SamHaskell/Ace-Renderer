#include <stdio.h>

#include "core/defines.hpp"
#include "core/app.hpp"

#include "maths/maths.hpp"
#include "graphics/graphicsdevice.hpp"
#include "graphics/mesh.hpp"

#include "imgui.h"

#include <algorithm>

namespace Ace {
    struct RenderFlags {
        bool WireFrame = false;
        bool Shaded = true;
        bool Culling = true;
        bool Vertices = false;
    };

    class AceRenderer : public App {
        public:
            AceRenderer() = default;
            ~AceRenderer() = default;

            void Initialise() override {
                m_CubeMesh = Mesh::Load("assets/f22.obj");
                m_CubeMesh->Position = { 0.0f, 0.0f, 6.0f };
                m_CubeMesh->Rotation = { -0.2f, 0.3f, 0.0f };
                m_CameraPosition = { 0.0f, 0.0f, 0.0f };
            }

            void Shutdown() override {
                delete m_CubeMesh;
            }

            void Update(f64 dt) override {
                m_CubeMesh->Rotation.y += 0.3f * dt;
                m_CubeMesh->Rotation.x += 0.3f * dt;
            }

            Vec2 ProjectOrthographic(Vec3 position) {
                return {
                    position.x,
                    position.y
                };
            }

            Vec2 ProjectPerspective(Vec3 position) {
                return {
                    position.x / position.z,
                    position.y / position.z
                };
            }

            void Render(PixelBuffer& pixelBuffer) override {
                Vec2 screenCenter = {(f32)pixelBuffer.Width / 2.0f, (f32)pixelBuffer.Height / 2.0f};
                
                m_TrianglesToRender.clear();

                pixelBuffer.Clear(0xFF111111);
                GraphicsDevice::DrawGrid(pixelBuffer, 0xFF333333, 64, 64);

                for (i32 i = 0; i < m_CubeMesh->Faces.size(); i++) {
                    Face face = m_CubeMesh->Faces[i];
                    Vec3 faceVerts[3] = {
                        m_CubeMesh->Vertices[face.a],
                        m_CubeMesh->Vertices[face.b],
                        m_CubeMesh->Vertices[face.c]
                    };

                    // Transform

                    Vec3 transformedVerts[3] = {
                        RotateZ(RotateY(RotateX(faceVerts[0], m_CubeMesh->Rotation.x), m_CubeMesh->Rotation.y), m_CubeMesh->Rotation.z) + m_CubeMesh->Position,
                        RotateZ(RotateY(RotateX(faceVerts[1], m_CubeMesh->Rotation.x), m_CubeMesh->Rotation.y), m_CubeMesh->Rotation.z) + m_CubeMesh->Position,
                        RotateZ(RotateY(RotateX(faceVerts[2], m_CubeMesh->Rotation.x), m_CubeMesh->Rotation.y), m_CubeMesh->Rotation.z) + m_CubeMesh->Position
                    };

                    // Cull

                    if (m_RenderFlags.Culling) {
                        Vec3 vec_ab = transformedVerts[1] - transformedVerts[0];
                        Vec3 vec_ac = transformedVerts[2] - transformedVerts[0];
                        Vec3 normal = Cross(vec_ab, vec_ac);
                        Vec3 camera = m_CameraPosition - transformedVerts[0];
                        if (Dot(normal, camera) <= 0.0f) {
                            continue;
                        }
                    }

                    transformedVerts[0] -= m_CameraPosition;
                    transformedVerts[1] -= m_CameraPosition;
                    transformedVerts[2] -= m_CameraPosition;

                    // Get depth-value for each triangle

                    f32 depth = (transformedVerts[0].z + transformedVerts[1].z + transformedVerts[2].z) / 3.0f;

                    // Project

                    m_TrianglesToRender.push_back(
                        {
                            .points = {
                                ProjectPerspective(transformedVerts[0]) * 920 + screenCenter,
                                ProjectPerspective(transformedVerts[1]) * 920 + screenCenter,
                                ProjectPerspective(transformedVerts[2]) * 920 + screenCenter
                            },
                            .depth = depth
                        }
                    );
                }

                // Sort triangles by depth (NOTE: std::sort for now, optimise later)

                std::sort(m_TrianglesToRender.begin(), m_TrianglesToRender.end(), [] (Triangle a, Triangle b) {
                    return a.depth > b.depth;
                });

                for (auto& triangle : m_TrianglesToRender) {
                    if (m_RenderFlags.Shaded) {
                        GraphicsDevice::DrawTriangleFill(
                            pixelBuffer,
                            0xFF777777,
                            triangle
                        );
                    }

                    if (m_RenderFlags.WireFrame) {
                        GraphicsDevice::DrawTriangle(
                            pixelBuffer,
                            0xFF000000,
                            triangle
                        );
                    }
                    
                    if (m_RenderFlags.Vertices) {
                        for (i32 j = 0; j < 3; j++) {
                            Rect rect = {
                                triangle.points[j].x - 2,
                                triangle.points[j].y - 2,
                                4,
                                4
                            };
                            GraphicsDevice::DrawRectFill(
                                pixelBuffer,
                                0xFFFFFF00,
                                rect
                            );  
                        }
                    }
                }
            }

            void DrawGUI() override {
                ImGui::Begin("Renderer Settings");
                ImGui::Checkbox("Wireframe", &m_RenderFlags.WireFrame);
                ImGui::Checkbox("Shaded", &m_RenderFlags.Shaded);
                ImGui::Checkbox("Backface Culling", &m_RenderFlags.Culling);
                ImGui::Checkbox("Vertices", &m_RenderFlags.Vertices);
                ImGui::End();
            }

            void OnEvent() override {

            }
            
        private:
            Vec3 m_CameraPosition;
            std::vector<Triangle> m_TrianglesToRender;
            Mesh* m_CubeMesh;
            RenderFlags m_RenderFlags;
    };
}

int main(int argc, char* argv[]) {
    Ace::AceRenderer* renderer = new Ace::AceRenderer();
    renderer->Run();
    return 0;
}