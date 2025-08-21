#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <shader.h>
#include <mesh.h>

class ResourceManager {
public:
    static int id;
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Mesh> meshes;

    // Shaders
    static Shader loadShader(std::string shader_name, std::string vertex, std::string fragment);
    static Shader getShader(std::string shader_name);

    // Meshs
    static Mesh loadMesh(std::string fileName);
    static Mesh getMesh(std::string fileName);
};

#endif