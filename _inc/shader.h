#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glm/fwd.hpp>

class Shader {
public:
    void compile(const std::string vertex, const std::string fragment);
    void setMatrix4(std::string name, const glm::mat4 &matrix);
    void setFloat(std::string name, float value);
    void setInt(std::string name, int value);

    unsigned int id;
};

#endif