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
                m_Mesh = Mesh::Load("assets/f117.obj");
                m_Mesh->Position.x = 0.0f;
                m_Texture = Texture::Load("assets/f117.png");
                m_DirectionalLight.Direction = Normalised({1.0f, -1.0f, 1.0f});
            }

            void Shutdown() override {
                delete m_Mesh;
                delete m_Texture;
            }

            void Update(f64 dt) override {
                m_DebugInfo.FrameTime = dt;
            }

            void Render(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer) override {
                Vec2 screenCenter = {(f32)pixelBuffer.Width / 2.0f, (f32)pixelBuffer.Height / 2.0f};
                f32 aspectRatio = (f32)pixelBuffer.Width / (f32)pixelBuffer.Height;
                
                pixelBuffer.Clear(0xFF111111);
                depthBuffer.Clear();

                m_TrianglesToRender.clear();
                m_TrianglesToClip.clear();
                m_TrianglesToProject.clear();

                Mat4 view = m_MainCamera.GetViewMatrix();

                Mat4 projection = Mat4::Perspective(
                    m_MainCamera.FovY,
                    aspectRatio, 
                    m_MainCamera.ZNear,
                    m_MainCamera.ZFar
                );

                // Send the Mesh through the pipeline.

                for (i32 i = 0; i < m_Mesh->Faces.size(); i++) {
                    Face face = m_Mesh->Faces[i];

                    Vec3 faceVerts[3] = {
                        m_Mesh->Vertices[face.a],
                        m_Mesh->Vertices[face.b],
                        m_Mesh->Vertices[face.c]
                    };

                    // Apply Model Transformation

                    Mat4 model = Mat4::Translation(m_Mesh->Position) * Mat4::Rotation(m_Mesh->Rotation) * Mat4::Scale(m_Mesh->Scale);

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

                    // Apply View Transformation

                    transformedVerts[0] = view * transformedVerts[0];
                    transformedVerts[1] = view * transformedVerts[1];
                    transformedVerts[2] = view * transformedVerts[2];

                    // Backface Culling

                    if (m_RenderFlags.Culling) {
                        if (Dot(normal, - Vec3(transformedVerts[0])) < 0.0f) {
                            continue;
                        }
                    }

                    Color color = {1.0, 1.0, 1.0, 1.0};

                    m_TrianglesToClip.push_back(
                        {   
                            .Vertices = {
                                {
                                    .Position = transformedVerts[0],
                                    .TexCoord = m_Mesh->TexCoords[face.aUV]
                                },
                                {
                                    .Position = transformedVerts[1],
                                    .TexCoord = m_Mesh->TexCoords[face.bUV]
                                    },
                                {
                                    .Position = transformedVerts[2],
                                    .TexCoord = m_Mesh->TexCoords[face.cUV]
                                }
                            },
                            .Color = color
                        }
                    );
                    
                }

                // Clipping
                for (auto& triangle : m_TrianglesToClip) {
                    
                    // Convert triangle to polygon for clipping.
                    Polygon polygon = {
                        .Vertices = {
                            triangle.Vertices[0], 
                            triangle.Vertices[1], 
                            triangle.Vertices[2]
                        },
                        .VertexCount = 3
                    };

                    // Clip against each frustum boundary plane.
                    for (i32 i = 0; i < FRUSTUM_PLANE_TYPE_COUNT; i++) {
                        Plane plane = m_MainCamera.GetFrustumPlane(FrustumPlaneType(i), aspectRatio);

                        Vertex innerVertices[MAX_POLY_VERTICES];
                        i32 innerVertexCount = 0;

                        Vertex* currentVertex = &polygon.Vertices[0];
                        Vertex* previousVertex = &polygon.Vertices[polygon.VertexCount - 1];

                        f32 currentSDF = 0.0f;
                        f32 previousSDF = Dot(Vec3(previousVertex->Position) - plane.Point, plane.Normal);

                        while (currentVertex != &polygon.Vertices[polygon.VertexCount]) {
                            currentSDF = Dot(Vec3(currentVertex->Position) - plane.Point, plane.Normal);

                            if ((currentSDF * previousSDF) < 0.0f) {
                                f32 t = previousSDF / (previousSDF - currentSDF);
                                ACE_INFO("%f", t);
                                Vec4 interpolatedPosition = Lerp(previousVertex->Position, currentVertex->Position, t);
                                Vec2 interpolatedTexCoord = Lerp(previousVertex->TexCoord, currentVertex->TexCoord, t);
                                innerVertices[innerVertexCount] = {
                                    .Position = interpolatedPosition,
                                    .TexCoord = interpolatedTexCoord
                                };
                                innerVertexCount ++;
                            }

                            if (currentSDF > 0.0f) {
                                innerVertices[innerVertexCount] = *currentVertex;
                                innerVertexCount ++;
                            }

                            previousSDF = currentSDF;
                            previousVertex = currentVertex;
                            currentVertex ++;
                        }

                        for (i32 j = 0; j < innerVertexCount; j++) {
                            polygon.Vertices[j] = innerVertices[j];
                        }
                        polygon.VertexCount = innerVertexCount;    
                    }

                    // Subdivide polygon into triangles to be projected.

                    if (polygon.VertexCount < 3) {
                        continue;
                    }

                    for (i32 i = 2; i < polygon.VertexCount; i++) {

                        Triangle tri = {
                            .Vertices = {
                                polygon.Vertices[0],
                                polygon.Vertices[i - 1],
                                polygon.Vertices[i]
                            },
                            .Color = triangle.Color
                        };

                        m_TrianglesToProject.push_back(tri);
                    }
                }
      
                for (auto& triangle : m_TrianglesToProject) {

                    // Apply Projection Transformation

                    Vec4 projectedVerts[3] = {
                        projection * triangle.Vertices[0].Position,
                        projection * triangle.Vertices[1].Position,
                        projection * triangle.Vertices[2].Position
                    };

                    // Perspective-Divide

                    for (i32 i = 0; i < 3; i++) {
                        projectedVerts[i].x = projectedVerts[i].x / projectedVerts[i].w;
                        projectedVerts[i].y = projectedVerts[i].y / projectedVerts[i].w;
                        projectedVerts[i].z = projectedVerts[i].z / projectedVerts[i].w;
                    }

                    // Shading TODO: Remove this if not continuing with lighting ...

                    // f32 lightingIntensity = - Dot(normal, m_DirectionalLight.Direction);
                    // lightingIntensity = Clamp(lightingIntensity + 0.3f, 0.0f, 1.0f);

                    // color.r *= lightingIntensity;
                    // color.g *= lightingIntensity;
                    // color.b *= lightingIntensity;

                    // Send triangle to be rasterised

                    m_TrianglesToRender.push_back(
                        {
                            .Vertices = {
                                {
                                    .Position = { (projectedVerts[0].x + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[0].y + 1.0f) * ((f32)pixelBuffer.Height / 2.0f), projectedVerts[0].z, projectedVerts[0].w },
                                    .TexCoord = { triangle.Vertices[0].TexCoord }
                                },
                                {
                                    .Position = { (projectedVerts[1].x + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[1].y + 1.0f) * ((f32)pixelBuffer.Height / 2.0f), projectedVerts[1].z, projectedVerts[1].w  },
                                    .TexCoord = { triangle.Vertices[1].TexCoord }
                                },
                                {
                                    .Position = { (projectedVerts[2].x + 1.0f) * ((f32)pixelBuffer.Width / 2.0f), (projectedVerts[2].y + 1.0f) * ((f32)pixelBuffer.Height / 2.0f), projectedVerts[2].z, projectedVerts[2].w  },
                                    .TexCoord = { triangle.Vertices[2].TexCoord }
                                }
                            },
                            .Color = triangle.Color
                        }
                    );
                }

                m_DebugInfo.TriangleCount = m_TrianglesToRender.size();

                // Draw!

                GraphicsDevice::DrawGrid(pixelBuffer, {0.12f, 0.12f, 0.12f, 1.0f}, 64, 64);

                for (auto& triangle : m_TrianglesToRender) {
                    if (m_RenderFlags.Shaded) {
                        GraphicsDevice::DrawTriangleTextured(
                            pixelBuffer,
                            depthBuffer,
                            *m_Texture,
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
                                {1.0, 0.6, 0.0, 1.0},
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

            void OnEvent(Event& e) override {
                switch (e.Type) {
                    case EventType::Key:
                        break;
                }
            }
            
        private:
            Camera m_MainCamera;
            std::vector<Triangle> m_TrianglesToClip;
            std::vector<Triangle> m_TrianglesToRender;
            std::vector<Triangle> m_TrianglesToProject;
            Mesh* m_Mesh;
            Texture* m_Texture;
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