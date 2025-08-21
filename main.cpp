#include <karia.h>
#include <print>

#include <emscripten.h>

void main_loop(void *karia) {
    auto *k = static_cast<Karia *>(karia);

    k->update();
    k->draw();
}

int main() {
    auto *k = new Karia();

    k->start();
    k->load();

    int fps = 0;
    emscripten_set_main_loop_arg(main_loop, k, fps, true);

    k->end();
    delete k;
}