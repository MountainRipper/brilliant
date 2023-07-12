#ifndef PLAYEREXAMPLE_H
#define PLAYEREXAMPLE_H
#include "brilliant-v/ui/mr_im_widget.h"
#include <tio/tio_types.h>
#include <mutex>
#include <queue>
#include <mrcommon/sdl_runner.h>
#include <mr_im_widget.h>
#include <yoya_lua_layout_context.h>
using namespace mr;


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

    YogaLuaLayoutContext yoga_layout_;

};

#endif // PLAYEREXAMPLE_H
