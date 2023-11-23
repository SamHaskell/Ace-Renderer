#include <stdio.h>

#include "core/defines.hpp"
#include "core/app.hpp"

#include "graphics/graphicsdevice.hpp"

class AceRenderer : public Ace::App {
    public:
        AceRenderer() = default;
        ~AceRenderer() = default;

        void Initialise() override {

        }

        void Update(f64 dt) override {

        }

        void Render(Ace::PixelBuffer& pixelBuffer) override {
            pixelBuffer.Clear(0xFF222222);
            Ace::GraphicsDevice::DrawGrid(pixelBuffer, 0xFF00FF00, 32, 32);
        }

        void OnEvent() override {

        }
        
    private:
        Ace::GraphicsDevice m_GraphicsDevice;
};

int main(int argc, char* argv[]) {
    AceRenderer* renderer = new AceRenderer();
    renderer->Run();
    return 0;
}