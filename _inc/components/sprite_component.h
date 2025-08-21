#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <shader.h>
#include <component.h>
#include <string>
#include <glm/glm.hpp>

class SpriteComponent : public Component {
public:
    SpriteComponent(std::string name, float w, float h);
    Shader shader;
    glm::vec2 size;
    unsigned int VAO;
};

#endif