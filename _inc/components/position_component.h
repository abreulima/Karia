#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include <component.h>
#include <glm/glm.hpp>

class PositionComponent : public Component {
public:
    PositionComponent(float x, float y, float z);
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif