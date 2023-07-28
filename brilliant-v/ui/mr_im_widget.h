#ifndef MRIMWIDGET_H
#define MRIMWIDGET_H

#include <functional>
#include <vector>
#include <string>
#include <map>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#if defined(MR_UI_WITH_TIO)
#include <tio/tio_types.h>
#endif

#include "basic_types.h"

namespace mrui
{


void ImRotateStart();
ImVec2 ImRotationCenter();
void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter());

void DragScrollCurrentWindow(bool &draged, int mouse_button = ImGuiMouseButton_Left, float release_speed = 1.0, float decelerate_factor = 0.9, bool scroll_y = true, bool scroll_x = false);

void Image(const char* image,const char* sub_image,const ImVec2& size, const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
void ImageRound(const char* image,const char* sub_image,const ImVec2& size, float round, const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
bool ImageButton(const char* str_id, const char* image,const char* sub_image, const ImVec2& size, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
bool RoundImageButton(const char* str_id, const char *image, const char *sub_image, const ImVec2 &size, const ImVec4 &tint_col, float round);

void RectangleFrame(const ImVec2& size, ImU32 color, float round = 0, float width = 1);
void Rectangle(const ImVec2& size, ImU32 color, float round = 0);

void CircelFrame(const ImVec2 &center,float radius, ImU32 color, float width = 1);
void Circel(const ImVec2 &center,float radius, ImU32 color);

//aligin_h,aligin_v 0=start,1=center,2=end
void TextAligined(const char* str, const ImVec2& size, int aligin_h, int aligin_v, ImU32 color);

class TextureHolder{
public:
    struct Image{

        ImVec2 top_left() const{
            return ImVec2(float(pos.x)/size_totle.x,float(pos.y)/size_totle.y);
        }
        ImVec2 bottom_right() const{
            return ImVec2(float(pos.x+size.x)/size_totle.x,float(pos.y+size.y)/size_totle.y);
        }

        ImVec2 pos;
        ImVec2 size;
        ImVec2 size_totle;
        ImTextureID texture = 0;
    };
    static Image get(const std::string& name);
    static int32_t add_image(const std::string&name, Image image);
    static const std::map<std::string,Image>& all_images();
#if defined(MR_UI_WITH_TIO)
    static int32_t add_image(const std::string& image_file);
    static std::map<std::string,mr::tio::GraphicTexture> textures_;
#endif
private:
    static std::map<std::string,Image> images_;
};


struct ImguiFontResource
{
    std::string ttf_file_path;
    const char* ttf_base85 = nullptr;
    void* ttf_binary_data = nullptr;
    uint32_t ttf_binary_size = 0;
    bool ttf_binary_compressed = false;
    float scale = 1.0;
    const ImWchar* range = nullptr;
    ImFontConfig config;
};

class ImGuiHelper{
public:
    static bool& pushed(){
        static bool s_pushed = false;
        return s_pushed;
    }
    static ImGuiHelper& get(const std::string& name = "defualt-font"){
        static std::map<std::string,ImGuiHelper> helpers;
        if(helpers.find(name) == helpers.end())
            helpers[name] = ImGuiHelper();
        return helpers[name];
    }
    void create_default_font(int32_t size,std::vector<ImguiFontResource> fonts){
        ImGuiIO& io = ImGui::GetIO();
        bool first = true;
        for(auto& item : fonts){
            item.config.MergeMode = !first;
            first = false;
            float font_size = size*item.scale;
            ImFont* font = nullptr;
            if(item.ttf_file_path.size()){
                font = io.Fonts->AddFontFromFileTTF(item.ttf_file_path.c_str(),font_size,&item.config,item.range);
            }
            else if(item.ttf_base85){
                font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(item.ttf_base85,font_size,&item.config,item.range);
            }
            else if(item.ttf_binary_data){
                if(item.ttf_binary_compressed)
                    font = io.Fonts->AddFontFromMemoryCompressedTTF(item.ttf_binary_data,item.ttf_binary_size,font_size,&item.config,item.range);
                else
                    font = io.Fonts->AddFontFromMemoryTTF(item.ttf_binary_data,item.ttf_binary_size,font_size,&item.config,item.range);
            }
            if(!font_ && font){
                font_ = font;
                origin_size_ = font_size;
                if(font_->FontSize == 0){
                    //load will not set FontSize?
                    font_->FontSize = origin_size_;
                }
            }
        }
    }

    void build(){
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Build();
    }

    int basic_font_size(){
        if(font_)
            return font_->FontSize;
        return ImGui::GetFont()->FontSize;
    }
    void use_font_size(float size){
        if(!font_)
            return;
        float scale = size / origin_size_;
        use_font_scale(scale);
    }

    void use_font_scale(float scale){
        if(!font_)
            return;
        if(pushed_font_){
            ImGui::PopFont();
            pushed() = false;
        }
        font_->Scale = scale;
        ImGui::PushFont(font_);
        pushed() = true;
    }

    void restore_font_size(){
        if(font_)
            font_->Scale = 1;
        ImGui::PopFont();
        pushed() = false;
    }
private:
    ImFont* font_ = nullptr;
    bool pushed_font_ = false;
    int origin_size_ = 0;
};


class ListView{
public:
    struct AbstractModel{
        size_t count(){
            return count_imp();
        }

        std::vector<std::string> keys(){
            return keys_imp();
        }

        CompatValue value(size_t index,const std::string_view& key){
            return value_imp(index,key);
        }

        void erase(size_t index){
            erase_imp(index);
        }
        virtual int count_imp(){
            return 0;
        }
        virtual std::vector<std::string> keys_imp(){
            return std::vector<std::string>();
        }
        virtual CompatValue value_imp(size_t index,const std::string_view& key){
            static CompatValue value = 0.0f;
            return value;
        }
        virtual void erase_imp(size_t index){

        }
    };
    //drawer callback function , return actual height the item used
    typedef std::function<int32_t(ListView& view,AbstractModel* model,int index,int x,int y,int width,int height)> draw_callback;
    ListView(){
        char temp[64];
        sprintf(temp,"mrui_listview_%p",this);
        id_ =  temp;
    }

    int32_t draw(AbstractModel* model, draw_callback drawer,int32_t width,int32_t height,int32_t item_height,int32_t space = 0)
    {
        ImGui::BeginChild(id_.c_str(),ImVec2(width,height),true);
        size_t count = model->count();
        int x = 0, y = 0;
        for(size_t index = 0; index < count; index++){
            y += drawer(*this,model,index,x,y,width,item_height);
        }
        DragScrollCurrentWindow(draded_);
        ImGui::EndChild();
        return 0;
    }

    int32_t update()
    {
        return 0;
    }
public:
    int32_t current_index_ = -1;
    bool draded_ = false;
private:
    std::string id_;
    int32_t width_;
    int32_t height_;
};


class VideoCutTimeline{
public:
    struct Fragment{


        void set_param(int duration_ms,int pixel_per_second){
            duration_ms_ = duration_ms;
            if(duration_ms_ == 0)
                return;
            if(range_.y == 0)
                range_ = ImVec2(0,duration_ms_);
            resize(pixel_per_second);
        }
        void resize(int pixel_per_second){
            if(range_.y == 0)
                range_ = ImVec2(0,duration_ms_);
            width_ = duration_ms_ / 1000.0 * pixel_per_second;
        }
        int32_t range_width(int pixel_per_second){
            return (range_.y - range_.x) / 1000.0 * pixel_per_second;
        }
        int duration_ms_ = 0;
        std::string name_;
        void* context_ = nullptr;
        ImVec2 range_;
        int width_ = 0;

        bool drag_start_ = false;
        bool drag_end_ = false;
    };
    VideoCutTimeline(){};

    void resize(int pixel_per_second){
        pixel_per_second_ = pixel_per_second;
        totle_width_ = 0;
        for(auto& item : fragments_){
            item.resize(pixel_per_second_);
            totle_width_ += item.width_;
        }
    }

    void insert(int index,const Fragment& fragment){
        if(fragments_.size() < index)
            return;
        fragments_.insert(fragments_.begin()+index,fragment)->resize(pixel_per_second_);
        resize(pixel_per_second_);
    }

    void remove(int index){
        if(fragments_.size() <= index)
            return;
        fragments_.erase(fragments_.begin()+index);
        resize(pixel_per_second_);
    }
    int size(){
        return fragments_.size();
    }
    void show(float height);
private:
    int pixel_per_second_ = 10;
    int range_holder_pixels_ = 20;
    int totle_width_ = 0;

    int offset_x_ = 0;
    bool mouse_down_ = false;
    std::vector<Fragment> fragments_;
    int select_item = 0;
};


}

#define IMGUI_CENTERED_CONTROL(control) mr::ControlCenterer{ImGui::GetWindowSize()}([&]() { control; })
#endif // MRIMWIDGET_H
