
#include "karia.h"
#include "keyboard_system.h"

#include "keyboard_component.h"
#include "position_component.h"

void KeyboardSystem::update(float delta_time, std::list<Entity> entities) {

    for (auto e : entities) {

        if (e.hasEntity<KeyboardComponent>()) {
            auto keyboard_component = e.getComponent<KeyboardComponent>();
            auto position_component = e.getComponent<PositionComponent>();

            float speed = 4.0f;

            if (Karia::key_state[SDL_SCANCODE_LEFT]) {
                position_component->position.x -= speed * delta_time;
            }
            if (Karia::key_state[SDL_SCANCODE_RIGHT]) {
                position_component->position.x += speed * delta_time;
            }
            if (Karia::key_state[SDL_SCANCODE_UP]) {
                position_component->position.z -= speed * delta_time;
            }
            if (Karia::key_state[SDL_SCANCODE_DOWN]) {
                position_component->position.z += speed * delta_time;
            }
        }

    }

}
