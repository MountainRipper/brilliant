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
