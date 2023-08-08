#define IMGUI_DEFINE_MATH_OPERATORS
#include <filesystem>
#include <thread>
#include <glad/gl.h>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/chrono.h>
#include <ttf/IconsFontAwesome6.h>
#include <ttf/ttf_notosans.h>
#include <mrcommon/filesystem.h>
#include "yoga_imgui_widget.h"
#include "example_mrui.h"


/*
 *Yoga :
 *Justify Content: hor layout for child
 */
MR_MR_SDL_RUNNER_SHOWCASE(MrUIExample)

using namespace mr::tio;

class TestModelData{
public:
    COMMON_PROPERTIES(double,id,std::string,name)
    COMPAT_PROPERTIES_EXPORT(TestModelData,id,name)
};
TypedListModel<TestModelData> test_model_;


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
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex((SDL_Window*)window),&mode);
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

    for (int index = 0; index < 128; ++index) {
        TestModelData data{(double)index,names[index%names.size()]};
        test_model_.append(data);
    }

    auto fonts = ImGui::GetIO().Fonts;
    ImFontConfig icons_config;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 1;
    icons_config.MergeMode = true;
    static const ImWchar ranges[] = { 0x0001, 0xFFFF, 0 };
    fonts->AddFontFromMemoryCompressedBase85TTF(notosans_compressed_data_base85,24,&icons_config,ranges);

    std::filesystem::path current_file(__FILE__);
    yoga_layout_context_.load((current_file.parent_path()/"resources"/"main.lua").string(),mode.w,mode.h);
    yoga_layout_context_.set_context_variant("testModel",(void*)&test_model_);

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
    ImGui::End();

    yoga_main_ui_->render_frame();
    yoga_record_ui_->render_frame();
}


