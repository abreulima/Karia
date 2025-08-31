#include "systems/render_system.h"

#include <iostream>
#include <GL/gl.h>
#include <SDL3/SDL.h>

#include "constans.h"

#include "resource_manager.h"
#include <systems/render_system.h>

#include "karia.h"
#include "model_component.h"
#include "position_component.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

Shader RenderSystem::full_quad_shader;
GLuint RenderSystem::g_buffer = 0;
GLuint RenderSystem::VAO = 0;
GLuint RenderSystem::g_color = 0;
GLuint RenderSystem::g_normal = 0;
GLuint RenderSystem::g_depth = 0;

void RenderSystem::init() {

    ResourceManager::loadShader("full_quad", "res/shaders/full_quad.vs", "res/shaders/full_quad.fs");
    full_quad_shader = ResourceManager::getShader("full_quad");

    float quad_vertices[] = {
        // pos      // uv
        -1,  1, 0.0, 1.0,
        -1, -1, 0.0, 0.0,
         1, -1, 1.0, 0.0,

        -1,  1, 0.0, 1.0,
         1, -1, 1.0, 0.0,
         1,  1, 1.0, 1.0
    };

    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenFramebuffers(1, &g_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

    // GL_RGBA16F, type HALF_FLOAT

    // Color
    glGenTextures(1, &g_color);
    glBindTexture(GL_TEXTURE_2D, g_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Karia::window_width, Karia::window_height, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_color, 0);

    // Normals
    glGenTextures(1, &g_normal);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Karia::window_width, Karia::window_height, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);

    // Depth
    // DEPTH_COMPONENT32F or 24?
    glGenTextures(1, &g_depth);
    glBindTexture(GL_TEXTURE_2D, g_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, Karia::window_width, Karia::window_height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    GL_TEXTURE_2D, g_depth, 0);

    GLenum attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderSystem::update(float delta_time, std::list<Entity> entities) {

    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);           // Make sure depth function is correct
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_TRUE);

    float zNear = 0.1f;
    float zFar = 25.f;

    for (auto e : entities) {

        if (e.hasEntity<ModelComponent>()) {
            auto model_component = e.getComponent<ModelComponent>();
            auto position_component = e.getComponent<PositionComponent>();

            glUseProgram(model_component->shader.id);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position_component->position);
            model = glm::rotate(model, glm::radians(position_component->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
            model = glm::rotate(model, glm::radians(position_component->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
            model = glm::rotate(model, glm::radians(position_component->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
            model = glm::scale(model, position_component->scale);

            // Dimetric View
            // Please Check ->
            float distance = 10.0f;
            float yaw = glm::radians(45.0f);
            float pitch = glm::radians(30.0f);

            glm::vec3 camera_pos;
            camera_pos.x = distance * cos(pitch) * sin(yaw);
            camera_pos.y = distance * sin(pitch);
            camera_pos.z = distance * cos(pitch) * cos(yaw);

            glm::mat4 view = glm::lookAt(camera_pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            // Move back
            /*
            glm::vec3 camera_position = glm::vec3(5.0f, 5.0f, 5.0f);
            glm::vec3 camera_target_position = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
            view = glm::lookAt(camera_position, camera_target_position, camera_up);
            */

            /*
            float base_scale = 8.0f;
            float resolution_factor = std::min(Karia::window_width / 1920.0f, Karia::window_height / 1080.0f);
            float scale = base_scale * resolution_factor;
            float aspect = static_cast<float>(Karia::window_width) / static_cast<float>(Karia::window_height);
            glm::mat4 projection = glm::ortho(-aspect * scale, aspect * scale, -scale, scale, zNear, zFar);
            */

            float scale = 8.0f;
            float aspect = static_cast<float>(Karia::window_width) / static_cast<float>(Karia::window_height);
            glm::mat4 projection = glm::ortho(-aspect * scale, aspect * scale, -scale, scale, zNear, zFar);

            model_component->shader.setMatrix4("model", model);
            model_component->shader.setMatrix4("view", view);
            model_component->shader.setMatrix4("projection", projection);

            glBindVertexArray(model_component->VAO);
            glDrawArrays(GL_TRIANGLES, 0, model_component->model.positions.size());
        }
    }

    // Full Quad Pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Karia::window_width, Karia::window_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(full_quad_shader.id);

    full_quad_shader.setInt("g_color", 0);
    full_quad_shader.setInt("g_normal", 1);
    full_quad_shader.setInt("g_depth", 2);
    full_quad_shader.setFloat("nearPlane", zNear);
    full_quad_shader.setFloat("farPlane", zFar);
    full_quad_shader.set2Float("resolution", Karia::window_width, Karia::window_height);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, g_color);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, g_normal);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, g_depth);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);

}

