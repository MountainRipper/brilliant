#ifndef PLAYEREXAMPLE_H
#define PLAYEREXAMPLE_H
#include "brilliant-v/ui/mr_im_widget.h"
#include <tio/tio_types.h>
#include <mutex>
#include <queue>
#include <mrcommon/sdl_runner.h>
#include <mr_im_widget.h>
#include <yoya_lua_layout_context.h>
#include <imgui_yoga_render.h>
#include <mrcommon/imgui_mr.h>
using namespace mr;



class VideoCutTimeline_{
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
    VideoCutTimeline_(){};

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

class MrUIExample : public SDLShowcaseBase
{
public:
    MrUIExample();

    // ExampleBase interface
public:
    virtual int32_t on_set_params(cxxopts::Options& options) override;
    virtual int32_t on_pre_init(cxxopts::ParseResult& options_result,uint32_t& window_flags) override;
    virtual int32_t on_init(void *window,int width, int height) override;
    virtual int32_t on_deinit() override;
    virtual int32_t on_frame() override;
    virtual void button_callback(int bt,int type,int clicks,double x,double y) override;
    virtual void cursor_callback(double x, double y) override;
    virtual void key_callback(int key, int type,int scancode,int mods) override;
    virtual void error_callback(int err, const char *desc) override;
    virtual void resize_callback(int width, int height) override;
    virtual void command(std::string command) override;
private:
    void render_ui();
private:

    //ui stuff
    int width_ = 0;
    int height_ = 0;
    bool resized_ = true;


    struct GridData
    {
        int index = 0;
        std::string name_;
    };

    ImFont* noto_font_ = nullptr;

    mrui::ListView<GridData> grid_;
    std::vector<GridData> grid_data_;
    std::string test_bundle_image_;
    bool button_window_draged_ = false;

    YogaLuaLayoutContext yoga_layout_context_;
    YogaLuaLayout* yoga_main_ui_;
    ImGuiYogaRender yoga_render_;

    VideoCutTimeline_ timeline_;
};

#endif // PLAYEREXAMPLE_H
