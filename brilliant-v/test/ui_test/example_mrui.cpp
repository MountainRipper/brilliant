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

#define SPECTRUM_USE_DARK_THEME
namespace Spectrum {
// a list of changes introduced to change the look of the widgets.
// Collected here as const rather than being magic numbers spread
// around imgui.cpp and imgui_widgets.cpp.
const float CHECKBOX_BORDER_SIZE = 2.0f;
const float CHECKBOX_ROUNDING = 2.0f;

// Load SourceSansProRegular and sets it as a default font.
// You may want to call ImGui::GetIO().Fonts->Clear() before this
void LoadFont(float size = 16.0f);

// Sets the ImGui style to Spectrum
void StyleColorsSpectrum();

namespace { // Unnamed namespace, since we only use this here.
unsigned int Color(unsigned int c) {
    // add alpha.
    // also swap red and blue channel for some reason.
    // todo: figure out why, and fix it.
    const short a = 0xFF;
    const short r = (c >> 16) & 0xFF;
    const short g = (c >> 8) & 0xFF;
    const short b = (c >> 0) & 0xFF;
    return(a << 24)
           | (r << 0)
           | (g << 8)
           | (b << 16);
}
}
// all colors are from http://spectrum.corp.adobe.com/color.html

inline unsigned int color_alpha(unsigned int alpha, unsigned int c) {
    return ((alpha & 0xFF) << 24) | (c & 0x00FFFFFF);
}

namespace Static { // static colors
const unsigned int NONE = 0x00000000; // transparent
const unsigned int WHITE = Color(0xFFFFFF);
const unsigned int BLACK = Color(0x000000);
const unsigned int GRAY200 = Color(0xF4F4F4);
const unsigned int GRAY300 = Color(0xEAEAEA);
const unsigned int GRAY400 = Color(0xD3D3D3);
const unsigned int GRAY500 = Color(0xBCBCBC);
const unsigned int GRAY600 = Color(0x959595);
const unsigned int GRAY700 = Color(0x767676);
const unsigned int GRAY800 = Color(0x505050);
const unsigned int GRAY900 = Color(0x323232);
const unsigned int BLUE400 = Color(0x378EF0);
const unsigned int BLUE500 = Color(0x2680EB);
const unsigned int BLUE600 = Color(0x1473E6);
const unsigned int BLUE700 = Color(0x0D66D0);
const unsigned int RED400 = Color(0xEC5B62);
const unsigned int RED500 = Color(0xE34850);
const unsigned int RED600 = Color(0xD7373F);
const unsigned int RED700 = Color(0xC9252D);
const unsigned int ORANGE400 = Color(0xF29423);
const unsigned int ORANGE500 = Color(0xE68619);
const unsigned int ORANGE600 = Color(0xDA7B11);
const unsigned int ORANGE700 = Color(0xCB6F10);
const unsigned int GREEN400 = Color(0x33AB84);
const unsigned int GREEN500 = Color(0x2D9D78);
const unsigned int GREEN600 = Color(0x268E6C);
const unsigned int GREEN700 = Color(0x12805C);
}

#ifdef SPECTRUM_USE_LIGHT_THEME
const unsigned int GRAY50 = Color(0xFFFFFF);
const unsigned int GRAY75 = Color(0xFAFAFA);
const unsigned int GRAY100 = Color(0xF5F5F5);
const unsigned int GRAY200 = Color(0xEAEAEA);
const unsigned int GRAY300 = Color(0xE1E1E1);
const unsigned int GRAY400 = Color(0xCACACA);
const unsigned int GRAY500 = Color(0xB3B3B3);
const unsigned int GRAY600 = Color(0x8E8E8E);
const unsigned int GRAY700 = Color(0x707070);
const unsigned int GRAY800 = Color(0x4B4B4B);
const unsigned int GRAY900 = Color(0x2C2C2C);
const unsigned int BLUE400 = Color(0x2680EB);
const unsigned int BLUE500 = Color(0x1473E6);
const unsigned int BLUE600 = Color(0x0D66D0);
const unsigned int BLUE700 = Color(0x095ABA);
const unsigned int RED400 = Color(0xE34850);
const unsigned int RED500 = Color(0xD7373F);
const unsigned int RED600 = Color(0xC9252D);
const unsigned int RED700 = Color(0xBB121A);
const unsigned int ORANGE400 = Color(0xE68619);
const unsigned int ORANGE500 = Color(0xDA7B11);
const unsigned int ORANGE600 = Color(0xCB6F10);
const unsigned int ORANGE700 = Color(0xBD640D);
const unsigned int GREEN400 = Color(0x2D9D78);
const unsigned int GREEN500 = Color(0x268E6C);
const unsigned int GREEN600 = Color(0x12805C);
const unsigned int GREEN700 = Color(0x107154);
const unsigned int INDIGO400 = Color(0x6767EC);
const unsigned int INDIGO500 = Color(0x5C5CE0);
const unsigned int INDIGO600 = Color(0x5151D3);
const unsigned int INDIGO700 = Color(0x4646C6);
const unsigned int CELERY400 = Color(0x44B556);
const unsigned int CELERY500 = Color(0x3DA74E);
const unsigned int CELERY600 = Color(0x379947);
const unsigned int CELERY700 = Color(0x318B40);
const unsigned int MAGENTA400 = Color(0xD83790);
const unsigned int MAGENTA500 = Color(0xCE2783);
const unsigned int MAGENTA600 = Color(0xBC1C74);
const unsigned int MAGENTA700 = Color(0xAE0E66);
const unsigned int YELLOW400 = Color(0xDFBF00);
const unsigned int YELLOW500 = Color(0xD2B200);
const unsigned int YELLOW600 = Color(0xC4A600);
const unsigned int YELLOW700 = Color(0xB79900);
const unsigned int FUCHSIA400 = Color(0xC038CC);
const unsigned int FUCHSIA500 = Color(0xB130BD);
const unsigned int FUCHSIA600 = Color(0xA228AD);
const unsigned int FUCHSIA700 = Color(0x93219E);
const unsigned int SEAFOAM400 = Color(0x1B959A);
const unsigned int SEAFOAM500 = Color(0x16878C);
const unsigned int SEAFOAM600 = Color(0x0F797D);
const unsigned int SEAFOAM700 = Color(0x096C6F);
const unsigned int CHARTREUSE400 = Color(0x85D044);
const unsigned int CHARTREUSE500 = Color(0x7CC33F);
const unsigned int CHARTREUSE600 = Color(0x73B53A);
const unsigned int CHARTREUSE700 = Color(0x6AA834);
const unsigned int PURPLE400 = Color(0x9256D9);
const unsigned int PURPLE500 = Color(0x864CCC);
const unsigned int PURPLE600 = Color(0x7A42BF);
const unsigned int PURPLE700 = Color(0x6F38B1);
#endif
#ifdef SPECTRUM_USE_DARK_THEME
const unsigned int GRAY50 = Color(0x252525);
const unsigned int GRAY75 = Color(0x2F2F2F);
const unsigned int GRAY100 = Color(0x323232);
const unsigned int GRAY200 = Color(0x393939);
const unsigned int GRAY300 = Color(0x3E3E3E);
const unsigned int GRAY400 = Color(0x4D4D4D);
const unsigned int GRAY500 = Color(0x5C5C5C);
const unsigned int GRAY600 = Color(0x7B7B7B);
const unsigned int GRAY700 = Color(0x999999);
const unsigned int GRAY800 = Color(0xCDCDCD);
const unsigned int GRAY900 = Color(0xFFFFFF);
const unsigned int BLUE400 = Color(0x2680EB);
const unsigned int BLUE500 = Color(0x378EF0);
const unsigned int BLUE600 = Color(0x4B9CF5);
const unsigned int BLUE700 = Color(0x5AA9FA);
const unsigned int RED400 = Color(0xE34850);
const unsigned int RED500 = Color(0xEC5B62);
const unsigned int RED600 = Color(0xF76D74);
const unsigned int RED700 = Color(0xFF7B82);
const unsigned int ORANGE400 = Color(0xE68619);
const unsigned int ORANGE500 = Color(0xF29423);
const unsigned int ORANGE600 = Color(0xF9A43F);
const unsigned int ORANGE700 = Color(0xFFB55B);
const unsigned int GREEN400 = Color(0x2D9D78);
const unsigned int GREEN500 = Color(0x33AB84);
const unsigned int GREEN600 = Color(0x39B990);
const unsigned int GREEN700 = Color(0x3FC89C);
const unsigned int INDIGO400 = Color(0x6767EC);
const unsigned int INDIGO500 = Color(0x7575F1);
const unsigned int INDIGO600 = Color(0x8282F6);
const unsigned int INDIGO700 = Color(0x9090FA);
const unsigned int CELERY400 = Color(0x44B556);
const unsigned int CELERY500 = Color(0x4BC35F);
const unsigned int CELERY600 = Color(0x51D267);
const unsigned int CELERY700 = Color(0x58E06F);
const unsigned int MAGENTA400 = Color(0xD83790);
const unsigned int MAGENTA500 = Color(0xE2499D);
const unsigned int MAGENTA600 = Color(0xEC5AAA);
const unsigned int MAGENTA700 = Color(0xF56BB7);
const unsigned int YELLOW400 = Color(0xDFBF00);
const unsigned int YELLOW500 = Color(0xEDCC00);
const unsigned int YELLOW600 = Color(0xFAD900);
const unsigned int YELLOW700 = Color(0xFFE22E);
const unsigned int FUCHSIA400 = Color(0xC038CC);
const unsigned int FUCHSIA500 = Color(0xCF3EDC);
const unsigned int FUCHSIA600 = Color(0xD951E5);
const unsigned int FUCHSIA700 = Color(0xE366EF);
const unsigned int SEAFOAM400 = Color(0x1B959A);
const unsigned int SEAFOAM500 = Color(0x20A3A8);
const unsigned int SEAFOAM600 = Color(0x23B2B8);
const unsigned int SEAFOAM700 = Color(0x26C0C7);
const unsigned int CHARTREUSE400 = Color(0x85D044);
const unsigned int CHARTREUSE500 = Color(0x8EDE49);
const unsigned int CHARTREUSE600 = Color(0x9BEC54);
const unsigned int CHARTREUSE700 = Color(0xA3F858);
const unsigned int PURPLE400 = Color(0x9256D9);
const unsigned int PURPLE500 = Color(0x9D64E1);
const unsigned int PURPLE600 = Color(0xA873E9);
const unsigned int PURPLE700 = Color(0xB483F0);
#endif
}
using namespace ImGui;
void StyleColorsSpectrum() {
    ImGuiStyle* style = &ImGui::GetStyle();
    style->GrabRounding = 4.0f;

    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Text] = ColorConvertU32ToFloat4(Spectrum::GRAY800); // text on hovered controls is gray900
    colors[ImGuiCol_TextDisabled] = ColorConvertU32ToFloat4(Spectrum::GRAY500);
    colors[ImGuiCol_WindowBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ColorConvertU32ToFloat4(Spectrum::GRAY50); // not sure about this. Note: applies to tooltips too.
    colors[ImGuiCol_Border] = ColorConvertU32ToFloat4(Spectrum::GRAY300);
    colors[ImGuiCol_BorderShadow] = ColorConvertU32ToFloat4(Spectrum::Static::NONE); // We don't want shadows. Ever.
    colors[ImGuiCol_FrameBg] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // this isnt right, spectrum does not do this, but it's a good fallback
    colors[ImGuiCol_FrameBgHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
    colors[ImGuiCol_FrameBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
    colors[ImGuiCol_TitleBg] = ColorConvertU32ToFloat4(Spectrum::GRAY300); // those titlebar values are totally made up, spectrum does not have this.
    colors[ImGuiCol_TitleBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
    colors[ImGuiCol_TitleBgCollapsed] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
    colors[ImGuiCol_MenuBarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
    colors[ImGuiCol_ScrollbarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100); // same as regular background
    colors[ImGuiCol_ScrollbarGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
    colors[ImGuiCol_ScrollbarGrabHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
    colors[ImGuiCol_ScrollbarGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
    colors[ImGuiCol_CheckMark] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
    colors[ImGuiCol_SliderGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
    colors[ImGuiCol_SliderGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY800);
    colors[ImGuiCol_Button] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // match default button to Spectrum's 'Action Button'.
    colors[ImGuiCol_ButtonHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
    colors[ImGuiCol_ButtonActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
    colors[ImGuiCol_Header] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
    colors[ImGuiCol_HeaderHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
    colors[ImGuiCol_HeaderActive] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
    colors[ImGuiCol_Separator] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
    colors[ImGuiCol_SeparatorHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
    colors[ImGuiCol_SeparatorActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
    colors[ImGuiCol_ResizeGrip] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
    colors[ImGuiCol_ResizeGripHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
    colors[ImGuiCol_ResizeGripActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
    colors[ImGuiCol_PlotLines] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
    colors[ImGuiCol_PlotLinesHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
    colors[ImGuiCol_PlotHistogram] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
    colors[ImGuiCol_PlotHistogramHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
    colors[ImGuiCol_TextSelectedBg] = ColorConvertU32ToFloat4((Spectrum::BLUE400 & 0x00FFFFFF) | 0x33000000);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ColorConvertU32ToFloat4((Spectrum::GRAY900 & 0x00FFFFFF) | 0x0A000000);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
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
    auto program_dir = mr::current_executable_dir();
    test_bundle_image_ = (std::filesystem::path(program_dir)/"icons.json").string();
    mrui::TextureHolder::add_image(test_bundle_image_);

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

    yoga_layout_context_.load("/home/xuwei/work/projects/MountainRipper/brilliant/brilliant-v/test/ui_test/resources/main.lua");
    yoga_main_ui_ = yoga_layout_context_.get_layout("org.mr.brilliant.MainUI");
    yoga_main_ui_->set_renderer(&yoga_render_);

    VideoCutTimeline_::Fragment f;
    f.set_param(10000,20);
    timeline_.insert(0,f);
    f.set_param(20000,20);
    timeline_.insert(0,f);
    f.set_param(8000,20);
    timeline_.insert(0,f);
    timeline_.resize(20);

    //StyleColorsSpectrum();
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

    timeline_.show(50);

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

}

using namespace ImGui;

void VideoCutTimeline_::show(float height)
{
    auto parent_window = GetCurrentWindow();
    PushID(this);
    ImVec2 size = ImVec2(totle_width_*2,height);

    SetCursorPosX(offset_x_);
    BeginChild("child",size,false,ImGuiWindowFlags_NoScrollbar);

    int index = 0;

    ImVec4 colors[] = {ImVec4{1,0.7,0.2,1},ImVec4{0.8,0.6,1,1},ImVec4{0.4,0.75,1,1},ImVec4{1,0.7,0.7,1}};

    std::vector<int> fragment_width_this_frame;
    bool refresh_width = false;
    index = 0;
    for(auto& item : fragments_){
        PushID(index);
        int range_width = item.range_width(pixel_per_second_);
        fragment_width_this_frame.push_back(range_width);
        ColorButton(item.name_.c_str(),colors[index],0,ImVec2(range_width,height));
        //InvisibleButton(item.name_.c_str(),ImVec2(item.width_,height));
        bool active = IsItemActive();
        if(active){
            if(IsMouseDown(0) && !mouse_down_){
                //need mark mouse_down_, only check first mouse down pos, so move mouse fast will not horver in head/tail area
                mouse_down_ = true;
                if(GetIO().MousePos.x >= GetItemRectMin().x && GetIO().MousePos.x <= GetItemRectMin().x + range_holder_pixels_){
                    if(!item.drag_start_)
                        fprintf(stderr,"Drag Start %s\n",item.name_.c_str());
                    item.drag_start_ = true;
                    refresh_width = true;
                }
                if(GetIO().MousePos.x >= GetItemRectMax().x - range_holder_pixels_ && GetIO().MousePos.x <= GetItemRectMax().x){
                    if(!item.drag_end_)
                        fprintf(stderr,"Drag End %s\n",item.name_.c_str());
                    item.drag_end_ = true;
                    refresh_width = true;
                }
                select_item = index;
            }
            if (IsMouseDragging(0)){
                int delta_x = GetIO().MouseDelta.x;
                if(item.drag_start_){
                    range_width -= delta_x;

                    if(range_width < pixel_per_second_*4)
                        range_width = pixel_per_second_*4;
                    else if(range_width > item.width_)
                        range_width = item.width_;
                    else
                        offset_x_ += delta_x;

                    float duration_ms = range_width * 1000.0 /  pixel_per_second_;
                    item.range_ = ImVec2(item.range_.y - duration_ms,item.range_.y);
                }
                else if(item.drag_end_){
                    range_width += delta_x;

                    if(range_width < pixel_per_second_*4)
                        range_width = pixel_per_second_*4;
                    else if(range_width > item.width_)
                        range_width = item.width_;

                    float duration_ms = range_width * 1000.0 /  pixel_per_second_;
                    item.range_ = ImVec2(item.range_.x, item.range_.x + duration_ms);
                }
                else{
                    offset_x_ += GetIO().MouseDelta.x;
                }
                fprintf(stderr,"%s select range:%f-%f\n",item.name_.c_str(),item.range_.x,item.range_.y);
            }
        }
        if(IsMouseReleased(0)){
            item.drag_start_ = false;
            item.drag_end_ = false;
            mouse_down_ = false;
            refresh_width = true;
        }

        SameLine(0,0);
        PopID();
        index++;
    }
    if(refresh_width){
        totle_width_ = 0;
        for(auto& item : fragments_){
            totle_width_ += item.width_;
        }
    }


    auto content_width = parent_window->Size.x;
    if(offset_x_ < -(totle_width_ - 200))
        offset_x_ = -(totle_width_ - 200);
    else if(offset_x_ > (content_width - 200) )
        offset_x_ = (content_width - 200);


    auto window = GetCurrentWindow();
    auto image_l = mrui::TextureHolder::get("timeline_handle_left.png");
    auto image_r = mrui::TextureHolder::get("timeline_handle_right.png");
    int32_t x = 0;
    index = 0;
    ImVec2 pos = window->Pos;
    for(auto& item : fragments_){
        if(select_item == index)
            window->DrawList->AddImage(image_l.texture,
                                       pos + ImVec2(x,0),
                                       pos + ImVec2(x+range_holder_pixels_,0+height),
                                       image_l.top_left(),
                                       image_l.bottom_right());

        x += fragment_width_this_frame[index];

        if(select_item == index)
            window->DrawList->AddImage(image_r.texture,
                                       pos + ImVec2(x-range_holder_pixels_,0),
                                       pos + ImVec2(x,0+height),
                                       image_r.top_left(),
                                       image_r.bottom_right());

        index++;
    }

    EndChild();
    PopID();

}
