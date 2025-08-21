#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <shader.h>
#include <model.h>

class ResourceManager {
public:
    static int id;
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Model> models;

    // Shaders
    static Shader loadShader(std::string shader_name, std::string vertex, std::string fragment);
    static Shader getShader(std::string shader_name);

    // Meshs
    static Model loadModel(std::string model_name, std::string model_file);
    static Model getModel(std::string model_name);
};

#endif