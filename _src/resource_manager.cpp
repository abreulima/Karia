#include <map>
#include <string>
#include <fstream>

#include <resource_manager.h>
#include <shader.h>
#include <mesh.h>
#include <GLES3/gl3.h>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Mesh> ResourceManager::meshes;

Shader ResourceManager::loadShader(std::string shader_name, std::string vertex, std::string fragment) {
    Shader shader;


    std::string vertex_shader_text =
    "attribute vec4 position;                      \
    void main()                                    \
    {                                              \
        gl_Position = vec4(vec3(0.0, 0.0, 0.0, 1.0);     \
    }                                              \
    ";

    std::string fragment_shader_text =
    "precision mediump float;                       \
    varying vec3 color;                            \
    void main()                                    \
    {                                              \
        gl_FragColor = vec4 (1.0);                  \
    }                                              \
    ";


    //std::ifstream ifsVertex(vertex);
    //std::string vertexSourceFromFile(std::istreambuf_iterator<char>{ifsVertex}, {});

    //std::ifstream ifsFragment(fragment);
    //std::string fragmentSourceFromFile(std::istreambuf_iterator<char>{ifsFragment}, {});

    shader.compile(vertex_shader_text, fragment_shader_text);
    shaders.insert(std::pair<std::string, Shader>(shader_name, shader));

    //ifsVertex.close();
    //ifsFragment.close();

    return shaders[shader_name];
}

Shader ResourceManager::getShader(std::string shader_name) {
    return shaders[shader_name];
}

Mesh ResourceManager::loadMesh(std::string fileName) {

}

Mesh ResourceManager::getMesh(std::string meshName) {

}