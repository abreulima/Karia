#include <karia.h>
#include <GLES3/gl3.h>

#include <entity.h>
#include <iostream>
#include <ostream>
#include <components/model_component.h>
#include <components/position_component.h>
#include <components/sprite_component.h>
#include <components/keyboard_component.h>
#include <systems/render_system.h>

#include <resource_manager.h>
#include <glm/gtc/matrix_transform.hpp>

#include "keyboard_system.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_opengl3.h"


int Karia::window_width;
int Karia::window_height;
std::map<SDL_Scancode, bool> Karia::key_state;

void Karia::start() {

    window_width = 1280;
    window_height = 720;

    window = SDL_CreateWindow("Karia", window_width, window_height, SDL_WINDOW_OPENGL);
    if (!window) {
        std::printf("Failed to create window: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::printf("Failed to create context: %s\n", SDL_GetError());
    }

    std::printf("INFO: GL Version %s\n", glGetString(GL_VERSION));
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplSDL3_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 300 es");


    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // Init Systems
    RenderSystem::init();

    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    // Primary background
    colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);  // #131318
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f); // #131318

    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);

    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.40f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);

    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.32f, 0.40f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.38f, 0.50f, 1.00f);

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.50f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.38f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.13f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);

    // Borders
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Text
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);

    // Highlights
    colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.70f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.70f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.80f, 1.00f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.70f, 1.00f, 0.50f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.80f, 1.00f, 0.75f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.90f, 1.00f, 1.00f);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.50f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.45f, 0.55f, 1.00f);

    // Style tweaks
    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.PopupBorderSize = 0.f;

    //
    is_running = true;
    currentScene = "scene_0";
}

void Karia::update() {

    last_update = now;
    now = SDL_GetTicks();
    delta_time = (now - last_update) / 1000.0f;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
            is_running = false;
        else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
            is_running = true;
        else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            window_width = event.window.data1;
            window_height = event.window.data2;
            RenderSystem::init();
        }
        else if (event.type == SDL_EVENT_KEY_DOWN) {
            key_state[event.key.scancode] = true;
        } else if(event.type == SDL_EVENT_KEY_UP) {
            key_state[event.key.scancode] = false;
        }
    }

    KeyboardSystem::update(delta_time, scenes[currentScene]);


}

void Karia::draw() {

    // TODO Calculate FPS

    RenderSystem::update(delta_time, scenes[currentScene]);

    static bool hide_GUI = false;


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();


    if (!hide_GUI) {

        ImGui::Begin("MyWindow");
        static const char* items[]{"scene_0","scene_1"};
        std::vector<std::string> scene_list = {"scene_0","scene_1"};
        static int Selecteditem = 0;
        static Entity selected_entity;
        bool check =  ImGui::Combo("Scenes", &Selecteditem, items, IM_ARRAYSIZE(items));
        ImGui::SeparatorText("Entities");
        if (check)
        {
            currentScene = items[Selecteditem];
            printf("Current Scene: %s\n", currentScene.c_str());
        }
        static bool is_fullscreen = false;

        ImGui::BeginChild("Entities0", ImVec2(0, 150));
        static int selected = -1;
        for ( const auto &e : scenes[currentScene] )
        {
            char buf[32];
            sprintf(buf, "%s", e.name.c_str());
            if (ImGui::Selectable(buf)) {

            }
        }
        ImGui::EndChild();

        ImGui::Checkbox("Fullscreen", &is_fullscreen);
        if (is_fullscreen) {
            SDL_SetWindowFullscreen(window, is_fullscreen);
        }

        ImGui::Checkbox("Hide Editor", &hide_GUI);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
        {
            ImGui::SetTooltip("Press T to toggle the editor.");
        }

        ImGui::SeparatorText("Hello X");
        ImGui::Text("Follow on");
        ImGui::SameLine();
        ImGui::TextLinkOpenURL("YouTube", "https://www.google.com/");

        ImGui::End();

        auto it = scenes.find(currentScene);
        selected_entity = it->second.front();

        ImGui::Begin("Entity");
        ImGui::Text("Entity Name");
        ImGui::SeparatorText("Transform");

        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
        const ImGuiSliderFlags flags_for_sliders = flags & ~ImGuiSliderFlags_WrapAround;
        if (selected_entity.hasEntity<PositionComponent>()) {
            auto pos_component = selected_entity.getComponent<PositionComponent>();

            ImGui::Text("Position");
            ImGui::DragFloat("X##Pos", &pos_component->position.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("Y##Pos", &pos_component->position.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("Z##Pos", &pos_component->position.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);

            ImGui::Text("Rotation");
            ImGui::SliderFloat("X##Rot", &pos_component->rotation.x, 0, 360.0f, "%.0f", flags);
            ImGui::SliderFloat("Y##Rot", &pos_component->rotation.y, 0, 360.0f, "%.0f", flags);
            ImGui::SliderFloat("Z##Rot", &pos_component->rotation.z, 0, 360.0f, "%.0f", flags);

            ImGui::Text("Scale");
            ImGui::DragFloat("X##Sca", &pos_component->scale.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("Y##Sca", &pos_component->scale.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
            ImGui::DragFloat("Z##Sca", &pos_component->scale.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
        }

        ImGui::SeparatorText("Model");
        ImGui::SeparatorText("Keyboard");

        ImGui::Separator();

        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("Add Component");

        }

        ImGui::End();



    }



    ImGui::Render();


    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    //glViewport(0, 0, 800, 600);
    //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void Karia::end() {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
void Karia::load() {


    // Shaders
    ResourceManager::loadShader("basic", "res/shaders/vertex.vs", "res/shaders/fragment.fs" );
    ResourceManager::loadShader("sprite", "res/shaders/vertex.vs", "res/shaders/fragment.fs" );
    // Textures

    // Models
    ResourceManager::loadModel("cube", "res/models/cube.obj");
    ResourceManager::loadModel("icosphere", "res/models/icosphere.obj");
    ResourceManager::loadModel("plane", "res/models/plane.obj");
    ResourceManager::loadModel("random", "res/models/random.obj");

    // Audios

    // Miscs.

    Entity player = Entity("player");
    player.addComponent<PositionComponent>(0.0f, 0.5f, 0.0f);
    player.addComponent<ModelComponent>("cube");
    player.addComponent<KeyboardComponent>();

    Entity plane = Entity("plane");
    plane.addComponent<PositionComponent>(0.0f, 0.0f, 0.0f);
    plane.addComponent<ModelComponent>("random");

    Entity icosphere = Entity();
    icosphere.addComponent<PositionComponent>(1.0f, 0.5f, 1.0f);
    icosphere.addComponent<ModelComponent>("icosphere");

    //player.addComponent<SpriteComponent>("assets/sprites/player.png", 20.f, 10.f);

    std::list<Entity> entities{player, plane};
    scenes.emplace("scene_0", entities);

    std::list<Entity> new_scene{ plane};
    scenes.emplace("scene_1", new_scene);

}



