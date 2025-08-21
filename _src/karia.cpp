#include <karia.h>
#include <cstdio>
#include <GLES3/gl3.h>

#include <entity.h>
#include <components/model_component.h>
#include <components/position_component.h>
#include <components/sprite_component.h>

#include <resource_manager.h>
#include <glm/gtc/matrix_transform.hpp>

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
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1);

}

void Karia::update() {

}

void Karia::draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto e : scenes["scene_0"]) {
        std::shared_ptr<ModelComponent> spr = e.getComponent<ModelComponent>();

        glUseProgram(spr->shader.id);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 view;
        glm::vec3 camera_position = glm::vec3(5.0f, 5.0f, 5.0f);
        glm::vec3 camera_target_position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
        view = glm::lookAt(camera_position, camera_target_position, camera_up);

        float zNear = 0.1f;
        float zFar = 20.f;
        float scale = 5.0f;
        float aspect = static_cast<float>(800) / static_cast<float>(600);
        glm::mat4 projection = glm::ortho(-aspect * scale, aspect * scale, -scale, scale, zNear, zFar);

        spr->shader.setMatrix4("model", model);
        spr->shader.setMatrix4("view", view);
        spr->shader.setMatrix4("projection", projection);

        glBindVertexArray(spr->VAO);
        glDrawArrays(GL_TRIANGLES, 0, spr->model.positions.size());
    }
    SDL_GL_SwapWindow(window);
}

void Karia::end() {

}
void Karia::load() {


    // Shaders
    ResourceManager::loadShader("basic", "res/shaders/vertex.vs", "res/shaders/fragment.fs" );
    ResourceManager::loadShader("sprite", "res/shaders/vertex.vs", "res/shaders/fragment.fs" );
    // Textures

    // Models
    ResourceManager::loadModel("cube", "res/models/cube.obj");

    // Audios

    // Miscs.

    Entity player = Entity("player");
    player.addComponent<PositionComponent>(0.3f, 0.5f, 0.5f);
    player.addComponent<ModelComponent>("cube");
    //player.addComponent<SpriteComponent>("assets/sprites/player.png", 20.f, 10.f);

    std::list<Entity> entities{player};
    scenes.emplace("scene_0", entities);

}



