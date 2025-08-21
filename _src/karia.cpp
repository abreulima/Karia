#include <karia.h>
#include <cstdio>
#include <GLES3/gl3.h>

#include <entity.h>
#include <components/position_component.h>
#include <components/sprite_component.h>

#include "resource_manager.h"

void Karia::start() {

    window = SDL_CreateWindow("Karia", 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::printf("Failed to create window: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::printf("Failed to create context: %s\n", SDL_GetError());
    }

    std::printf("INFO: GL Version %s\n", glGetString(GL_VERSION));
    SDL_GL_SetSwapInterval(1);

}

void Karia::update() {

}

void Karia::draw() {
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto e : scenes["scene_0"]) {
        std::shared_ptr<SpriteComponent> spr = e.getComponent<SpriteComponent>();

        //glUseProgram(spr->shader.id);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    SDL_GL_SwapWindow(window);
}

void Karia::end() {

}
void Karia::load() {

   ResourceManager::loadShader("sprite", "res/shaders/vertex.vs", "res/shaders/fragment.fs" );


    Entity player = Entity("player");
    player.addComponent<PositionComponent>(0.3f, 0.5f, 0.5f);
    //player.addComponent<SpriteComponent>("assets/sprites/player.png", 20.f, 10.f);

    std::list<Entity> entities{player};
    scenes.emplace("scene_0", entities);

}



