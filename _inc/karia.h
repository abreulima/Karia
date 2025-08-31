#ifndef KARIA_H
#define KARIA_H

#ifdef __EMSCRIPTEN__
    #include <emscripten/em_types.h>
#endif

#include <string>
#include <list>
#include <map>

#include <SDL3/SDL.h>
#include <entity.h>

class Karia {
public:
    SDL_Window *window;
    static int window_width;
    static int window_height;
    SDL_GLContext context;
    SDL_Event event;
    static std::map<SDL_Scancode, bool> key_state;
    std::string currentScene;
    std::map<std::string, std::list<Entity>> scenes;
    uint32_t now = 0;
    uint32_t last_update = 0;
    double delta_time = 0;
    bool is_running;
    void start();
    void load();
    void update();
    void draw();
    void end();
};

#endif