#ifndef KARIA_H
#define KARIA_H

#include <string>
#include <list>
#include <map>
#include <emscripten/em_types.h>
#include <SDL3/SDL.h>
#include <entity.h>

class Karia {
public:
    SDL_Window *window;
    SDL_GLContext context;
    SDL_Event event;
    std::map<SDL_Scancode, bool> key_state;
    std::string currentScene = "scene_0";
    std::map<std::string, std::list<Entity>> scenes;
    bool is_running;
    void start();
    void load();
    void update();
    void draw();
    void end();
};

#endif