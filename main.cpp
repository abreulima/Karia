#include <karia.h>
#include <print>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

void main_loop(void *karia) {
    auto *k = static_cast<Karia *>(karia);

    k->update();
    k->draw();
}

int main() {
    auto *k = new Karia();

    k->start();
    k->load();

    #ifdef __EMSCRIPTEN__
        int fps = 0;
        emscripten_set_main_loop_arg(main_loop, k, fps, true);
    #else
        while(k->is_running) {
            k->update();
            k->draw();
        }
    #endif

    k->end();
    delete k;
}