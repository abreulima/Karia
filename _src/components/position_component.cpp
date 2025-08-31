#include <components/position_component.h>

PositionComponent::PositionComponent(float x, float y, float z) {
   this->position = glm::vec3(x, y, z);
   this->rotation = glm::vec3(0, 0, 0);
   this->scale = glm::vec3(1, 1, 1);
}
