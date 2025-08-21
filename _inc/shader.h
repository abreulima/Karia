#ifndef SHADER_H
#define SHADER_H
#include <string>

class Shader {
public:
    void compile(const std::string vertex, const std::string fragment);
    unsigned int id;
};

#endif