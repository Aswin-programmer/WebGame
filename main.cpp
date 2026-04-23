#include <emscripten.h>
#include <memory>
#include "Engine.h"

std::unique_ptr<Engine> engine;

int main() {
    engine = std::make_unique<Engine>();

    emscripten_set_main_loop([]() {
        engine->MainLoop();
    }, 0, true);

    return 0;
}