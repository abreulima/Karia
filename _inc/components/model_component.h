#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H


#include <string>
#include <component.h>
#include <model.h>
#include <shader.h>

class ModelComponent : public Component {
  public:
    ModelComponent(std::string model_name);
    std::string model_name;
    unsigned int VAO;
    Model model;
    Shader shader;

};


#endif