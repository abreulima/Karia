#ifndef KARIA_RENDER_SYSTEM_H
#define KARIA_RENDER_SYSTEM_H

#include <entity.h>
#include <GLES3/gl3.h>

#include "shader.h"
#include "system.h"

class RenderSystem : public System {
    public:
        static void init();
        static void update(float delta_time, std::list<Entity> entities);

        // G-Buffer
        static Shader full_quad_shader;
        static GLuint g_buffer;
        static GLuint VAO;
        static GLuint g_color, g_normal, g_depth;
};


#endif //KARIA_RENDER_SYSTEM_H