#include <stdio.h>

#include "core/defines.hpp"
#include "core/app.hpp"

#include "maths/maths.hpp"
#include "graphics/graphicsdevice.hpp"
#include "graphics/mesh.hpp"

namespace Ace {
    class AceRenderer : public App {
        public:
            AceRenderer() = default;
            ~AceRenderer() = default;

            void Initialise() override {
                m_CameraPosition = { 0.0f, 0.0f, -3.0f };
                m_CubeRotation = {0.0f, 0.0f, 0.0f};
            }

            void Update(f64 dt) override {
                ACE_INFO("Frame Time: %.3lf ms", dt * 1000.0);
                m_CubeRotation.y += 0.1f * dt;
                m_CubeRotation.x += 0.05f * dt;
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
                
                pixelBuffer.Clear(0xFF111111);
                GraphicsDevice::DrawGrid(pixelBuffer, 0xFF333333, 64, 64);

                for (i32 i = 0; i < MESH_FACE_COUNT; i++) {
                    Face face = g_MeshFaces[i];
                    Vec3 faceVerts[3] = {
                        g_MeshVertices[face.a],
                        g_MeshVertices[face.b],
                        g_MeshVertices[face.c]
                    };

                    Vec3 transformedVerts[3] = {
                        RotateZ(RotateY(RotateX(faceVerts[0], m_CubeRotation.x), m_CubeRotation.y), m_CubeRotation.z) - m_CameraPosition,
                        RotateZ(RotateY(RotateX(faceVerts[1], m_CubeRotation.x), m_CubeRotation.y), m_CubeRotation.z) - m_CameraPosition,
                        RotateZ(RotateY(RotateX(faceVerts[2], m_CubeRotation.x), m_CubeRotation.y), m_CubeRotation.z) - m_CameraPosition
                    };

                    m_TrianglesToRender[i] = {
                        ProjectPerspective(transformedVerts[0]) * 160 + screenCenter,
                        ProjectPerspective(transformedVerts[1]) * 160 + screenCenter,
                        ProjectPerspective(transformedVerts[2]) * 160 + screenCenter
                    };
                }

                for (i32 i = 0; i < MESH_FACE_COUNT; i++) {
                    Triangle triangle = m_TrianglesToRender[i];
                    for (i32 j = 0; j < 3; j++) {
                        Rect rect = {
                            triangle.points[j].x,
                            triangle.points[j].y,
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

            void OnEvent() override {

            }
            
        private:
            Vec3 m_CameraPosition;
            Vec3 m_CubeRotation;
            Triangle m_TrianglesToRender[MESH_FACE_COUNT];
    };
}

int main(int argc, char* argv[]) {
    Ace::AceRenderer* renderer = new Ace::AceRenderer();
    renderer->Run();
    return 0;
}