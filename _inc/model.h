#ifndef PROJECTS_MESH_H
#define PROJECTS_MESH_H

#include <vector>
#include <glm/glm.hpp>

class Model {
    public:
    Model() {};

    unsigned int id;
    unsigned int VAO;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> colors;
};

#endif //PROJECTS_MESH_H