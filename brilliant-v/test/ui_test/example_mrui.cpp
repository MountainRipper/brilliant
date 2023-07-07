#define IMGUI_DEFINE_MATH_OPERATORS
#include <filesystem>
#include <glad/gl.h>
#include <imgui.h>
#include <mrcommon/imgui_mr.h>
#include <thread>
#include <ttf/IconsFontAwesome6.h>
#include <libavutil/frame.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/chrono.h>
#include <SDL2/SDL.h>
#include "../../ui/mr_im_widget.h"
#include "example_mrui.h"

MR_MR_SDL_RUNNER_SHOWCASE(MrUIExample)

using namespace mr::tio;

#include "imgui_internal.h"
void ScrollWhenDraggingOnVoid(const ImVec2& delta, ImGuiMouseButton mouse_button)
{
    ImGuiContext& g = *ImGui::GetCurrentContext();
    ImGuiWindow* window = g.CurrentWindow;
    bool hovered = false;
    bool held = false;
    ImGuiButtonFlags button_flags = (mouse_button == 0) ? ImGuiButtonFlags_MouseButtonLeft : (mouse_button == 1) ? ImGuiButtonFlags_MouseButtonRight : ImGuiButtonFlags_MouseButtonMiddle;
    if (g.HoveredId == 0) // If nothing hovered so far in the frame (not same as IsAnyItemHovered()!)
        ImGui::ButtonBehavior(window->Rect(), window->GetID("##scrolldraggingoverlay"), &hovered, &held, button_flags);

    held = ImGui::IsMouseHoveringRect(window->Rect().GetTL(),window->Rect().GetBR());

    held = held & ImGui::GetIO().MouseDown[0];
    if (held && delta.x != 0.0f)
        ImGui::SetScrollX(window, window->Scroll.x + delta.x);
    if (held && delta.y != 0.0f)
        ImGui::SetScrollY(window, window->Scroll.y + delta.y);
}

MrUIExample::MrUIExample()
{
}


int32_t MrUIExample::on_set_params(cxxopts::Options &options)
{
    return 0;
}

int32_t MrUIExample::on_pre_init(cxxopts::ParseResult &options_result, uint32_t &window_flags)
{
    return 0;
}
int32_t MrUIExample::on_init(void *window,int width, int height)
{
    width_ = width;
    height_ = height;
    test_bundle_image_ = "E:\\projects\\MountainRipper\\build-brilliant-Desktop_Qt_6_5_1_MinGW_64_bit-Debug\\bin\\images.json";
    mrui::TextureHolder::add_image(test_bundle_image_);
    return 0;
}

int32_t MrUIExample::on_deinit()
{

    return 0;
}

int32_t MrUIExample::on_frame()
{
    glClearColor(1.0,0.5,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_ui();
    return 0;
}

void MrUIExample::button_callback(int bt, int type, int clicks, double x, double y)
{

}


void MrUIExample::cursor_callback(double x, double y)
{
}

void MrUIExample::key_callback(int key, int type, int scancode, int mods)
{

}

void MrUIExample::error_callback(int err, const char *desc)
{
}

void MrUIExample::resize_callback(int width, int height)
{
    width_ = width;
    height_ = height;
    resized_ = true;
}

void MrUIExample::command(std::string command)
{

}


void MrUIExample::render_ui()
{
    ImGui::Begin("Button Panel",NULL);
    auto& images = mrui::TextureHolder::all_images();
    for(auto& image : images){
        ImGui::PushStyleColor(ImGuiCol_Button,0);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImGui::ColorConvertFloat4ToU32(ImVec4(0.8,0.8,0.8,0.5)));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImGui::ColorConvertFloat4ToU32(ImVec4(0.8,0.8,0.8,0.8)));
        bool clicked = mrui::ImageButton(image.first.c_str(),image.first.c_str(),NULL,image.second.size);
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        mrui::Image(image.first.c_str(),NULL,image.second.size);

        if(clicked ){
            if(!button_window_draged_){
                MR_INFO("Button {} clicked",image.first);
            }
        }
    }
    static float last_delta = 0;
    ImVec2 mouse_delta = ImGui::GetIO().MouseDelta;
    ScrollWhenDraggingOnVoid(ImVec2(0.0f, -mouse_delta.y), ImGuiMouseButton_Left);
    if(!button_window_draged_){
        button_window_draged_ = (mouse_delta.y != 0) && ImGui::GetIO().MouseDown[0];
        if(ImGui::GetIO().MouseDown[0])
            last_delta = 0;
    }
    if(ImGui::GetIO().MouseReleased[0]){
        button_window_draged_ = false;
        last_delta = -mouse_delta.y*2;
    }

    if(last_delta != 0){
        ImGuiContext& g = *ImGui::GetCurrentContext();
        ImGuiWindow* window = g.CurrentWindow;
        last_delta *= 0.9;
        if(abs(last_delta) < 1)
            last_delta = 0;
        MR_INFO("drag smooth {}",last_delta);
        ImGui::SetScrollY(window, window->Scroll.y + last_delta);

        if(window->Scroll.y == 0 || window->Scroll.y == window->ScrollMax.y){
            last_delta = 0;
        }
    }
    ImGui::End();


}
