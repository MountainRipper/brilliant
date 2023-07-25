#define IMGUI_DEFINE_MATH_OPERATORS
#include <filesystem>
#include <glad/gl.h>
#include <imgui.h>
#include <mrcommon/filesystem.h>
#include <thread>
#include <ttf/IconsFontAwesome6.h>
#include <libavutil/frame.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/chrono.h>
#include <SDL2/SDL.h>
#include "example_mrui.h"

#include <ttf/ttf_notosans.h>

/*
 *Yoya :
 *Justify Content: hor layout for child
 */
MR_MR_SDL_RUNNER_SHOWCASE(MrUIExample)

using namespace mr::tio;


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
    //SDL_GL_SetSwapInterval(0);
    width_ = width;
    height_ = height;
    auto program_dir = mr::current_executable_dir();
    test_bundle_image_ = (std::filesystem::path(program_dir)/"icons.json").string();
    mrui::TextureHolder::add_image(test_bundle_image_);
    auto video_editing_image_ = (std::filesystem::path(program_dir)/"video-editing.json").string();
    mrui::TextureHolder::add_image(video_editing_image_);

    const std::vector<std::string> names =  {"Flavio","Nicolás","Üzeyir","Йордан","ГЕОРГИЕВА","Διαμαντόπουλος","Csongrádi Ildikó",
                                  "Đoàn Diệu","संसाधन ओओर","Надежда Владимиров","张三","李四","蔡徐坤","鸡你太美"};

    for (int index = 0; index < 100; ++index) {

        GridData data{index,names[index%names.size()]};
        grid_data_.push_back(data);
    }

    auto fonts = ImGui::GetIO().Fonts;
    ImFontConfig icons_config;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 1;
    icons_config.MergeMode = true;
    static const ImWchar ranges[] = { 0x0001, 0xFFFF, 0 };
    fonts->AddFontFromMemoryCompressedBase85TTF(notosans_compressed_data_base85,24,&icons_config,ranges);
#if defined(__linux__)
    yoga_layout_context_.load("/home/xuwei/work/projects/MountainRipper/brilliant/brilliant-v/test/ui_test/resources/main.lua");
#elif defined(__WIN32)
    yoga_layout_context_.load("E:\\projects\\MountainRipper\\brilliant\\brilliant-v\\test\\ui_test\\resources\\main.lua");
#endif
    yoga_main_ui_ = yoga_layout_context_.get_layout("org.mr.brilliant.MainUI");
    yoga_main_ui_->set_renderer(&yoga_render_);
    yoga_record_ui_ = yoga_layout_context_.get_layout("org.mr.brilliant.RecordUI");
    yoga_record_ui_->set_renderer(&yoga_render_);

    mrui::VideoCutTimeline::Fragment f;
    f.set_param(10000,20);
    timeline_.insert(0,f);
    f.set_param(20000,20);
    timeline_.insert(0,f);
    f.set_param(8000,20);
    timeline_.insert(0,f);
    timeline_.resize(20);

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

unsigned long GetTickCount()

{

    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);

}
void ImRotateDemo()
{
    mrui::ImRotateStart();
    ImGui::TextColored(ImVec4(1,1,0,0.5), __FUNCTION__);
    mrui::ImRotateEnd(0.0005f*::GetTickCount());

    mrui::ImRotateStart();
    ImGui::Text(__FUNCTION__);ImGui::SameLine();
    ImGui::Text(ICON_FA_CABLE_CAR);
    mrui::ImRotateEnd(0.01f*::GetTickCount()*!ImGui::IsItemHovered());
}


void MrUIExample::render_ui()
{
    static bool first = true;
    if(first){
        first = false;
    }
    ImGui::Begin("Button Panel",NULL);

   // timeline_.show(50);

    ImGui::BeginChild("Bt",ImVec2(800,800),true,ImGuiWindowFlags_HorizontalScrollbar);
    auto& images = mrui::TextureHolder::all_images();
    ImGui::PushStyleColor(ImGuiCol_Button,0);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImGui::ColorConvertFloat4ToU32(ImVec4(0.8,0.8,0.8,0.5)));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImGui::ColorConvertFloat4ToU32(ImVec4(0.8,0.8,0.8,0.8)));
    float a = 0.01;
    for(auto& image : images){
        bool clicked = mrui::ImageButton(image.first.c_str(),image.first.c_str(),NULL,image.second.size);

        ImGui::SameLine();
        mrui::Image(image.first.c_str(),NULL,image.second.size,ImVec4(1,1,1,a));
        a += 0.02;
        if(clicked ){
            if(!button_window_draged_){
                MR_INFO("Button {} clicked",image.first);
            }
        }
    }
    ImGui::PopStyleColor(3);
    mrui::DragScrollCurrentWindow(button_window_draged_,ImGuiMouseButton_Left,1,0.9,true,true);
    ImGui::EndChild();

    ImGui::SameLine();

    if(noto_font_)
        ImGui::PushFont(noto_font_);

    int clicked_index = -1;

    grid_.draw(grid_data_,
        [this,&clicked_index](mrui::ListView<GridData>& view,GridData& data,int index,int width,int height){

            float start_y = ImGui::GetCursorPosY();
            ImGui::Spacing();
            ImGui::Text("%d",index);ImGui::SameLine();
            ImGui::SetCursorPosX(40);
            mrui::Image(test_bundle_image_.c_str(),"blueman-trust.png",ImVec2(height,height));ImGui::SameLine();
            auto pos_logo = ImGui::GetCursorPos();

            ImGui::PushID(index);

            ImGui::SameLine();
            ImGui::SetCursorPosX(width - ImGui::CalcTextSize(data.name_.c_str()).x - 40);

            if(ImGui::ButtonEx(data.name_.c_str()) && !view.draded_){
                clicked_index = index;
            }


            ImRect rect;
            ImGui::ItemSize(rect);
            //ImGui::SameLine();

            ImGui::SetCursorPos(pos_logo);
            if(ImGui::InvisibleButton("coverButton",ImVec2(rect.Min.x - pos_logo.x,height)) && !view.draded_){
                view.current_index_ = data.index;
            }

            ImGui::PopID();

            ImGui::Spacing();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

            float end_y = ImGui::GetCursorPosY();
            ImGui::SetCursorPosX(0);
            ImGui::SetCursorPosY(start_y);
            if(view.current_index_ == data.index){
                mrui::Image("select-border.png",NULL,ImVec2(width,end_y-start_y));ImGui::SameLine();
            }
            ImGui::SetCursorPosY(end_y);
            return 0;
    },800,800,40);

    if(noto_font_){
        ImGui::PopFont();
    }
    if(clicked_index >= 0){
        grid_data_.erase(grid_data_.begin()+clicked_index);
        if(grid_.current_index_ == clicked_index)
            grid_.current_index_ = -1;
    }

    //ImRotateDemo();

    ImGui::End();

    yoga_main_ui_->render_frame();
    yoga_record_ui_->render_frame();
}
