#include <map>
#include <string>
#include <fstream>

#include <resource_manager.h>
#include <shader.h>
#include <model.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>

#include "tiny_obj_loader.h"


std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Model> ResourceManager::models;

Shader ResourceManager::loadShader(std::string shader_name, std::string vertex, std::string fragment) {
    Shader shader;

    std::ifstream ifsVertex(vertex);
    std::string vertexSourceFromFile(std::istreambuf_iterator<char>{ifsVertex}, {});

    std::ifstream ifsFragment(fragment);
    std::string fragmentSourceFromFile(std::istreambuf_iterator<char>{ifsFragment}, {});

    shader.compile(vertexSourceFromFile, fragmentSourceFromFile);
    shaders.insert(std::pair<std::string, Shader>(shader_name, shader));

    ifsVertex.close();
    ifsFragment.close();

    return shaders[shader_name];
}

Shader ResourceManager::getShader(std::string shader_name) {
    return shaders[shader_name];
}

Model ResourceManager::loadModel(std::string model_name, std::string model_file) {
    Model model;

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;
    std::string warning;

    if (!tinyobj::LoadObj(
		    &attributes,
		    &shapes,
		    &materials,
		    &warning,
		    &error,
		    model_file.c_str())) {
		    throw std::runtime_error("ast::assets::loadOBJFile: Error: " + error + warning);
		    }

    for (const auto &shape: shapes) {
        for (const auto &index: shape.mesh.indices) {
            glm::vec3 position{
                attributes.vertices[3 * index.vertex_index + 0],
                attributes.vertices[3 * index.vertex_index + 1],
                attributes.vertices[3 * index.vertex_index + 2]
            };
            model.positions.push_back(position);

            if (index.texcoord_index >= 0) {
                glm::vec2 texcoords{
                    attributes.texcoords[2 * index.texcoord_index + 0],
                    attributes.texcoords[2 * index.texcoord_index + 1]
                };
                model.tex_coords.push_back(texcoords);
            }

            if (index.normal_index >= 0) {
                glm::vec3 normals{
                    attributes.normals[3 * index.normal_index + 0],
                    attributes.normals[3 * index.normal_index + 1],
                    attributes.normals[3 * index.normal_index + 2]

                };
                model.normals.push_back(normals);
            }

            glm::vec3 colors{
                attributes.colors[3 * index.vertex_index + 0],
                attributes.colors[3 * index.vertex_index + 1],
                attributes.colors[3 * index.vertex_index + 2]
            };
            model.colors.push_back(colors);

        }
    }
    std::printf("%f \n", model.colors[0][0]);
    models[model_name] = model;
    return models[model_name];
}

Model ResourceManager::getModel(std::string model_name) {
    return models[model_name];
}