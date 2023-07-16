#ifndef IMGUIYOGARENDER_H
#define IMGUIYOGARENDER_H
#include "yoga_lua_layout.h"

class ImGuiYogaRender : public YogaLuaLayoutRenderer
{
public:
    ImGuiYogaRender();

    // YogaLuaLayoutRenderer interface
public:
    virtual int32_t on_render_elements(YogaElement &element) override;
    virtual int32_t after_render_elements(YogaElement &element) override;
    int32_t element_push_style_var(YogaElement &element, int32_t &pushed_style_color, int32_t &pushed_style_var);
};

#endif // IMGUIYOGARENDER_H
