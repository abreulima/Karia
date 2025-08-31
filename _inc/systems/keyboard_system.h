#ifndef KARIA_KEYBOARD_SYSTEM_H
#define KARIA_KEYBOARD_SYSTEM_H

#include <entity.h>
#include "system.h"

class KeyboardSystem : public System {
public:
    static void init();
    static void update(float delta_time, std::list<Entity> entities);
};

#endif //KARIA_KEYBOARD_SYSTEM_H