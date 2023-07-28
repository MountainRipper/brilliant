#include "basic_types.h"
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
 *Yoga :
 *Justify Content: hor layout for child
 */
MR_MR_SDL_RUNNER_SHOWCASE(MrUIExample)

using namespace mr::tio;


class TestModelData{
public:
    GETTER_SETTER(double,id)
    GETTER_SETTER(std::string,name)
};

#define MODEL_EXPORT_PROPERTY(name) \
keys_.push_back(#name)

template<typename DataType>
class TestModel: public mrui::ListView::AbstractModel{
    // AbstractModel interface
public:
    virtual int count_imp() override{
        return data_vector_.size();
    }
    virtual std::vector<std::string> keys_imp() override{
        return std::vector<std::string>();
    }
    virtual CompatValue value_imp(size_t index, const std::string_view &key) override{
        DataType& data = data_vector_[index];
        GETTER_RETURN(data,id,key)
        GETTER_RETURN(data,name,key)
        return CompatValue();
    }
    virtual void erase_imp(size_t index) override{
        if(index >= data_vector_.size())
            return;
        data_vector_.erase(data_vector_.begin()+index);
    }
    void append(DataType& data){
        data_vector_.push_back(data);
    }

    typedef std::map<std::string,CompatValue> CompatProperties;
    std::vector<DataType> data_vector_;
    std::vector<std::string> keys_;
};
TestModel<TestModelData> test_model_;

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

    for (int index = 0; index < 100; ++index) {

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

    /*if(noto_font_)
        ImGui::PushFont(noto_font_);

    int clicked_index = -1;

    grid_.draw(&test_model_,
        [this,&clicked_index](mrui::ListView& view,mrui::ListView::AbstractModel* model,int index,int x,int y,int width,int height){
            std::string name = CompatValue(model->value(index,"name"));
            int32_t data_index = CompatValue(model->value(index,"id"));

            float start_y = ImGui::GetCursorPosY();
            ImGui::Spacing();
            ImGui::Text("%d",data_index);ImGui::SameLine();
            ImGui::SetCursorPosX(40);
            mrui::Image(test_bundle_image_.c_str(),"blueman-trust.png",ImVec2(height,height));ImGui::SameLine();
            auto pos_logo = ImGui::GetCursorPos();

            ImGui::PushID(index);

            ImGui::SameLine();

            ImGui::SetCursorPosX(width - ImGui::CalcTextSize(name.c_str()).x - 40);

            if(ImGui::ButtonEx(name.c_str()) && !view.draded_){
                clicked_index = index;
            }


            ImRect rect;
            ImGui::ItemSize(rect);
            //ImGui::SameLine();

            ImGui::SetCursorPos(pos_logo);
            if(ImGui::InvisibleButton("coverButton",ImVec2(rect.Min.x - pos_logo.x,height)) && !view.draded_){
                view.current_index_ = data_index;
            }

            ImGui::PopID();

            ImGui::Spacing();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

            float end_y = ImGui::GetCursorPosY();
            ImGui::SetCursorPosX(0);
            ImGui::SetCursorPosY(start_y);
            if(view.current_index_ == data_index){
                mrui::Image("select-border.png",NULL,ImVec2(width,end_y-start_y));ImGui::SameLine();
            }
            ImGui::SetCursorPosY(end_y);
            return 0;
    },800,800,40);

    if(noto_font_){
        ImGui::PopFont();
    }
    if(clicked_index >= 0){
        test_model_.erase(clicked_index);
        if(grid_.current_index_ == clicked_index)
            grid_.current_index_ = -1;
    }*/

    //ImRotateDemo();


    ImGui::End();

    yoga_main_ui_->render_frame();
    yoga_record_ui_->render_frame();
}


