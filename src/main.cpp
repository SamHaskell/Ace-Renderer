#include <stdio.h>

#include "core/defines.hpp"
#include "core/app.hpp"

#include "maths/maths.hpp"
#include "graphics/graphicsdevice.hpp"
#include "graphics/mesh.hpp"
#include "graphics/color.hpp"
#include "graphics/texture.hpp"
#include "graphics/camera.hpp"

#include "imgui.h"

#include <algorithm>

namespace Ace {
    struct DebugInfo {
        f64 FrameTime = 0.0;
        u32 TriangleCount = 0;
    };

    struct DirectionalLight {
        Vec3 Direction;
    };

    struct RenderFlags {
        bool WireFrame = false;
        bool Shaded = true;
        bool Culling = true;
        bool Vertices = false;
        bool ShowDepthBuffer = false;
    };

    class AceRenderer : public App {
        public:
            AceRenderer() = default;
            ~AceRenderer() = default;

            void Initialise() override {
                m_CubeMesh = Mesh::Load("assets/crab.obj");
                m_CubeMesh->Position = { 0.0f, 0.0f, 0.0f };
                m_CubeMesh->Rotation = { 0.0f, 0.0f, 0.0f };

                m_DirectionalLight.Direction = Normalised({1.0f, -1.0f, 1.0f});

                m_CubeTexture = Texture::Load("assets/crab.png");
            }

            void Shutdown() override {
                delete m_CubeMesh;
            }

            void Update(f64 dt) override {
                m_CubeMesh->Rotation += {0.0f * (f32)dt, 0.0f * (f32)dt, 0.0f * (f32)dt};
                m_DebugInfo.FrameTime = dt;
            }

            void Render(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer) override {
                Vec2 screenCenter = {(f32)pixelBuffer.Width / 2.0f, (f32)pixelBuffer.Height / 2.0f};
                
                m_TrianglesToRender.clear();

                pixelBuffer.Clear(0xFF111111);
                depthBuffer.Clear();

                GraphicsDevice::DrawGrid(pixelBuffer, {0.4f, 0.4f, 0.4f, 1.0f}, 64, 64);

                Mat4 view = m_MainCamera.GetViewMatrix();

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

                    Vec3 vec_ab = Vec3(transformedVerts[1]) - Vec3(transformedVerts[0]);
                    Vec3 vec_ac = Vec3(transformedVerts[2]) - Vec3(transformedVerts[0]);
                    Vec3 normal = Cross(vec_ab, vec_ac);
                    normal.NormaliseInPlace();
                    if (m_RenderFlags.Culling) {
                        Vec3 camera = m_MainCamera.Position - Vec3(transformedVerts[0]);
                        if (Dot(normal, camera) < 0.0f) {
                            continue;
                        }
                    }

                    transformedVerts[0] = view * transformedVerts[0];
                    transformedVerts[1] = view * transformedVerts[1];
                    transformedVerts[2] = view * transformedVerts[2];

                    // Project

                    Mat4 proj = Mat4::Perspective(
                        m_MainCamera.FovY,
                        (f32)pixelBuffer.Width / (f32)pixelBuffer.Height, 
                        m_MainCamera.ZNear,
                        m_MainCamera.ZFar
                    );

                    Vec4 projectedVerts[3] = {
                        proj * transformedVerts[0],
                        proj * transformedVerts[1],
                        proj * transformedVerts[2]
                    };

                    // Perspective-Divide

                    for (i32 i = 0; i < 3; i++) {
                        projectedVerts[i].x = projectedVerts[i].x / projectedVerts[i].w;
                        projectedVerts[i].y = projectedVerts[i].y / projectedVerts[i].w;
                        projectedVerts[i].z = projectedVerts[i].z / projectedVerts[i].w;
                    }

                    // Shading

                    f32 lightingIntensity = - Dot(normal, m_DirectionalLight.Direction);
                    lightingIntensity = Clamp(lightingIntensity + 0.3f, 0.0f, 1.0f);

                    Color color = {0.2, 0.2, 0.8, 1.0};

                    color.r *= lightingIntensity;
                    color.g *= lightingIntensity;
                    color.b *= lightingIntensity;

                    // Send triangle to be rastered

                    m_TrianglesToRender.push_back(
                        {
                            .Vertices = {
                                {
                                    .Position = { (projectedVerts[0].x + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[0].y + 1.0f) * ((f32)pixelBuffer.Height / 2.0f), projectedVerts[0].z, projectedVerts[0].w },
                                    .TexCoord = { m_CubeMesh->TexCoords[face.aUV] }
                                },
                                {
                                    .Position = { (projectedVerts[1].x + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[1].y + 1.0f) * ((f32)pixelBuffer.Height / 2.0f), projectedVerts[1].z, projectedVerts[1].w  },
                                    .TexCoord = { m_CubeMesh->TexCoords[face.bUV] }
                                },
                                {
                                    .Position = { (projectedVerts[2].x + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[2].y + 1.0f) * ((f32)pixelBuffer.Height / 2.0f), projectedVerts[2].z, projectedVerts[2].w  },
                                    .TexCoord = { m_CubeMesh->TexCoords[face.cUV] }
                                }
                            },
                            .Color = color
                        }
                    );
                }

                m_DebugInfo.TriangleCount = m_TrianglesToRender.size();

                for (auto& triangle : m_TrianglesToRender) {
                    if (m_RenderFlags.Shaded) {
                        GraphicsDevice::DrawTriangleTextured(
                            pixelBuffer,
                            depthBuffer,
                            *m_CubeTexture,
                            triangle
                        );
                    }

                    if (m_RenderFlags.WireFrame) {
                        GraphicsDevice::DrawTriangle(
                            pixelBuffer,
                            depthBuffer,
                            {0.0, 1.0, 0.0, 1.0},
                            triangle
                        );
                    }
                    
                    if (m_RenderFlags.Vertices) {
                        for (i32 j = 0; j < 3; j++) {
                            Rect rect = {
                                triangle.Vertices[j].Position.x - 2,
                                triangle.Vertices[j].Position.y - 2,
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
                
                if (m_RenderFlags.ShowDepthBuffer) {
                    for (i32 i = 0; i < pixelBuffer.Width; i++) {
                        for (i32 j = 0; j < pixelBuffer.Height; j++) {
                            f32 depthVal = depthBuffer.GetValue(i, j);
                            Color depthColor = {depthVal, depthVal, depthVal, 1.0f};
                            pixelBuffer.SetPixel(i, j, depthColor.U32_RGBA());
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
                ImGui::Checkbox("Overlay Depth Buffer", &m_RenderFlags.ShowDepthBuffer);
                ImGui::End();

                ImGui::Begin("Debug Information");
                ImGui::Text("%-12s: %-.4lf ms", "Frame Time", m_DebugInfo.FrameTime * 1000.0);
                ImGui::Text("%-12s: %-d", "Tri Count", m_DebugInfo.TriangleCount);
                ImGui::End();
            }

            void OnEvent() override {

            }
            
        private:
            Camera m_MainCamera;
            std::vector<Triangle> m_TrianglesToRender;
            Mesh* m_CubeMesh;
            Texture* m_CubeTexture;
            RenderFlags m_RenderFlags;
            DirectionalLight m_DirectionalLight;
            DebugInfo m_DebugInfo;
    };
}

int main(int argc, char* argv[]) {
    Ace::AceRenderer* renderer = new Ace::AceRenderer();
    renderer->Run();
    return 0;
}