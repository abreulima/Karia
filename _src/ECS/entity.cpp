#include <entity.h>
#include <string>

Entity::Entity() {
    isActive = true;
}

Entity::Entity(std::string name) {
    isActive = true;
    this->name = name;
}
