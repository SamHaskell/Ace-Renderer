#include <stdio.h>

#include "core/defines.hpp"
#include "core/app.hpp"

class AceRenderer : public Ace::App {
    public:
        AceRenderer() = default;
        ~AceRenderer() = default;

        void Initialise() override {

        }

        void Update(f64 dt) override {

        }

        void Render() override {

        }

        void OnEvent() override {
            
        }
        
    private:
};

int main(int argc, char* argv[]) {
    AceRenderer* renderer = new AceRenderer();
    renderer->Run();
    return 0;
}