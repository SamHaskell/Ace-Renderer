#include <stdio.h>

#include "core/defines.hpp"
#include "core/app.hpp"

#include "maths/maths.hpp"
#include "graphics/graphicsdevice.hpp"

namespace Ace {
    class AceRenderer : public App {
        public:
            AceRenderer() = default;
            ~AceRenderer() = default;

            void Initialise() override {
                for (i32 i = 0; i < 21; i++) {
                    for (i32 j = 0; j < 21; j++) {
                        for (i32 k = 0; k < 21; k++) {
                            m_CubePoints[(k * 21 * 21) + (j * 21) + i] = {
                                .x = (i - 10) * 0.03f,
                                .y = (j - 10) * 0.03f,
                                .z = (k - 10) * 0.03f
                            };
                        }
                    }
                }

                m_CameraPosition = { 0.0f, 0.0f, -1.0f };

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
                pixelBuffer.Clear(0xFF222222);

                for (i32 i = 0; i < 21 * 21 * 21; i++) {
                    Vec3 rotatedPosition = RotateX(RotateY(RotateZ(m_CubePoints[i], m_CubeRotation.z), m_CubeRotation.y), m_CubeRotation.x);
                    m_ProjectedPoints[i] = ProjectPerspective(rotatedPosition - m_CameraPosition);
                }

                for (i32 i = 0; i < 21 * 21 * 21; i++) {
                    pixelBuffer.SetPixel(
                        m_ProjectedPoints[i].x * 640 + (pixelBuffer.Width / 2), 
                        m_ProjectedPoints[i].y * 640 + (pixelBuffer.Height / 2), 
                        0xFF00FF00
                    );
                }

            }

            void OnEvent() override {

            }
            
        private:
            Vec3 m_CameraPosition;
            Vec3 m_CubeRotation;
            Vec3 m_CubePoints[21 * 21 * 21];
            Vec2 m_ProjectedPoints[21 * 21 * 21];
    };
}

int main(int argc, char* argv[]) {
    Ace::AceRenderer* renderer = new Ace::AceRenderer();
    renderer->Run();
    return 0;
}