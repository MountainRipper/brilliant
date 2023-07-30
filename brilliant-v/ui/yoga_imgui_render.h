#ifndef IMGUIYOGARENDER_H
#define IMGUIYOGARENDER_H
#include "yoga_lua_layout.h"
#include "imgui_widget_mr.h"

class ImGuiYogaRender : public YogaLuaLayoutRenderer
{
public:
    ImGuiYogaRender();

    struct StyleSetter{
        void(*color_setter_uint32)(ImGuiCol idx, ImU32 col);
        void(*color_setter_vec4)(ImGuiCol idx, const ImVec4& col);
        void(*var_setter_float)(ImGuiStyleVar idx, float val);
        void(*var_setter_vec2)(ImGuiStyleVar idx, const ImVec2& val);
    };
    int32_t attach_environment(sol::state& lua_state);
    int32_t load_theme(const std::string& theme_name = "defaultTheme");
    int32_t element_push_style_var(YogaElement &element, int32_t &pushed_style_color, int32_t &pushed_style_var,StyleSetter* setter = nullptr);
    // YogaLuaLayoutRenderer interface
public:
    virtual int32_t on_render_elements(YogaElement &element) override;
    virtual int32_t after_render_elements(YogaElement &element) override;
private:
    StyleSetter default_push_setter_{&ImGui::PushStyleColor,&ImGui::PushStyleColor,&ImGui::PushStyleVar,&ImGui::PushStyleVar};
    bool theme_loaded_ = false;
    std::map<YogaElement*,std::shared_ptr<mrui::ListView>> listviews_;
};

#endif // IMGUIYOGARENDER_H
