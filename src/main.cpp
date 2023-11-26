#include <stdio.h>

#include "core/defines.hpp"
#include "core/app.hpp"

#include "maths/maths.hpp"
#include "graphics/graphicsdevice.hpp"
#include "graphics/mesh.hpp"
#include "graphics/color.hpp"

#include "imgui.h"

#include <algorithm>

namespace Ace {
    struct DirectionalLight {
        Vec3 Direction;
    };

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
                m_CubeMesh->Position = { 0.0f, 0.0f, 0.0f };
                m_CubeMesh->Rotation = { 0.0f, 0.0f, 0.0f };
                m_CameraPosition = { 0.0f, 0.0f, -5.0f };

                m_DirectionalLight.Direction = {1.0f, -1.0f, 1.0f};
            }

            void Shutdown() override {
                delete m_CubeMesh;
            }

            void Update(f64 dt) override {
                m_CubeMesh->Rotation += {20.0f * (f32)dt, 30.0f * (f32)dt, 40.0f * (f32)dt};
            }

            Vec2 ProjectOrthographic(Vec3 position) {
                return {
                    position.x,
                    position.y
                };
            }

            Vec2 ProjectPerspective(Vec4 position) {
                return {
                    position.x / position.z,
                    position.y / position.z
                };
            }

            void Render(PixelBuffer& pixelBuffer) override {
                Vec2 screenCenter = {(f32)pixelBuffer.Width / 2.0f, (f32)pixelBuffer.Height / 2.0f};
                
                m_TrianglesToRender.clear();

                pixelBuffer.Clear(0xFF111111);
                GraphicsDevice::DrawGrid(pixelBuffer, {0.4f, 0.4f, 0.4f, 1.0f}, 64, 64);

                for (i32 i = 0; i < m_CubeMesh->Faces.size(); i++) {
                    Face face = m_CubeMesh->Faces[i];
                    Vec3 faceVerts[3] = {
                        m_CubeMesh->Vertices[face.a],
                        m_CubeMesh->Vertices[face.b],
                        m_CubeMesh->Vertices[face.c]
                    };

                    // Transform

                    Mat4 model = Mat4::Translation(m_CubeMesh->Position) * Mat4::Rotation(m_CubeMesh->Rotation) * Mat4::Scale(m_CubeMesh->Scale);

                    Vec4 transformedVerts[3] = {
                        model * faceVerts[0],
                        model * faceVerts[1],
                        model * faceVerts[2]
                    };

                    // Cull

                    if (m_RenderFlags.Culling) {
                        Vec3 vec_ab = Vec3(transformedVerts[1]) - Vec3(transformedVerts[0]);
                        Vec3 vec_ac = Vec3(transformedVerts[2]) - Vec3(transformedVerts[0]);
                        Vec3 normal = Cross(vec_ab, vec_ac);
                        Vec3 camera = m_CameraPosition - Vec3(transformedVerts[0]);
                        if (Dot(normal, camera) < 0.0f) {
                            continue;
                        }
                    }

                    Mat4 view = Mat4::Translation( - m_CameraPosition);

                    transformedVerts[0] = view * transformedVerts[0];
                    transformedVerts[1] = view * transformedVerts[1];
                    transformedVerts[2] = view * transformedVerts[2];

                    // Get depth-value for each triangle

                    f32 depth = (transformedVerts[0].z + transformedVerts[1].z + transformedVerts[2].z) / 3.0f;

                    // Project

                    Mat4 proj = Mat4::Perspective(60.0f, (f32)pixelBuffer.Width / (f32)pixelBuffer.Height, 0.1f, 100.0f);

                    Vec4 projectedVerts[3] = {
                        proj * transformedVerts[0],
                        proj * transformedVerts[1],
                        proj * transformedVerts[2]
                    };

                    m_TrianglesToRender.push_back(
                        {
                            .points = {
                                { (projectedVerts[0].x / projectedVerts[0].w + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[0].y / projectedVerts[0].w + 1.0f) * ((f32)pixelBuffer.Height / 2.0f) },
                                { (projectedVerts[1].x / projectedVerts[1].w + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[1].y / projectedVerts[1].w + 1.0f) * ((f32)pixelBuffer.Height / 2.0f) },
                                { (projectedVerts[2].x / projectedVerts[2].w + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[2].y / projectedVerts[2].w + 1.0f) * ((f32)pixelBuffer.Height / 2.0f) }
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
                            {0.6, 0.6, 0.6, 1.0},
                            triangle
                        );
                    }

                    if (m_RenderFlags.WireFrame) {
                        GraphicsDevice::DrawTriangle(
                            pixelBuffer,
                            {0.0, 1.0, 0.0, 1.0},
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
                                {1.0, 1.0, 0.0, 1.0},
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
            DirectionalLight m_DirectionalLight;
    };
}

int main(int argc, char* argv[]) {
    Ace::AceRenderer* renderer = new Ace::AceRenderer();
    renderer->Run();
    return 0;
}