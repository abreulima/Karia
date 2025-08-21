#include <iostream>
#include <ostream>
#include <shader.h>
#include <string>
#include <GLES3/gl3.h>

void Shader::compile (std::string vertex_file, std::string fragment_file) {
    unsigned int vertex_id;
    unsigned int fragment_id;

    // Vertex Shader
    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertex_src = vertex_file.c_str();
    glShaderSource(vertex_id, 1, &vertex_src, NULL);
    glCompileShader(vertex_id);
    GLint success;
    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &length);
        char *info_log = new char[length];
        glGetShaderInfoLog(vertex_id, length, &length, &info_log[0]);
        std::cout << info_log << std::endl;
    }

    // Fragment Shader
    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragment_src = fragment_file.c_str();
    glShaderSource(fragment_id, 1, &fragment_src, NULL);
    glCompileShader(fragment_id);
    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &length);
        char *info_log = new char[length];
        glGetShaderInfoLog(fragment_id, length, &length, &info_log[0]);
        std::cout << info_log << std::endl;
    }

    // Shader Program
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_id);
    glAttachShader(this->id, fragment_id);
    glLinkProgram(this->id);
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        int length;
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &length);
        char *info_log = new char[length];
        glGetProgramInfoLog(this->id, length, &length, &info_log[0]);
        std::cout << info_log << std::endl;
    }
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
}
