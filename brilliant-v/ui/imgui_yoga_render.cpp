
#define IMGUI_DEFINE_MATH_OPERATORS
#include "mr_im_widget.h"
#include <imgui.h>
#include <filesystem>
#include <mrcommon/logger.h>
#include <mrcommon/filesystem.h>
#include <sol/sol.hpp>
#include "imgui_yoga_render.h"


#define SWAP_ENDIAN(u32) u32 = (u32 >> 24) | ((u32<<8) & 0x00FF0000) | ((u32>>8) & 0x0000FF00) | (u32 << 24);

struct ImGuiStyleVarInfo{
    uint8_t value_count;
    ImGuiStyleVar_ type;
};
std::unordered_map<std::string,ImGuiStyleVarInfo> map_style_var_name = {
    // name ---------------------// Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
    {"alpha",                   {1,ImGuiStyleVar_Alpha} },                    // float     Alpha
    {"disabledAlpha",           {1,ImGuiStyleVar_DisabledAlpha} },            // float     DisabledAlpha
    {"windowPadding",           {2,ImGuiStyleVar_WindowPadding} },            // ImVec2    WindowPadding
    {"windowRounding",          {1,ImGuiStyleVar_WindowRounding} },           // float     WindowRounding
    {"windowBorderSize",        {1,ImGuiStyleVar_WindowBorderSize} },         // float     WindowBorderSize
    {"windowMinSize",           {2,ImGuiStyleVar_WindowMinSize} },            // ImVec2    WindowMinSize
    {"windowTitleAlign",        {2,ImGuiStyleVar_WindowTitleAlign} },         // ImVec2    WindowTitleAlign
    {"childRounding",           {1,ImGuiStyleVar_ChildRounding} },            // float     ChildRounding
    {"childBorderSize",         {1,ImGuiStyleVar_ChildBorderSize} },          // float     ChildBorderSize
    {"popupRounding",           {1,ImGuiStyleVar_PopupRounding} },            // float     PopupRounding
    {"popupBorderSize",         {1,ImGuiStyleVar_PopupBorderSize} },          // float     PopupBorderSize
    {"framePadding",            {2,ImGuiStyleVar_FramePadding} },             // ImVec2    FramePadding
    {"frameRounding",           {1,ImGuiStyleVar_FrameRounding} },            // float     FrameRounding
    {"frameBorderSize",         {1,ImGuiStyleVar_FrameBorderSize} },          // float     FrameBorderSize
    {"itemSpacing",             {2,ImGuiStyleVar_ItemSpacing} },              // ImVec2    ItemSpacing
    {"itemInnerSpacing",        {2,ImGuiStyleVar_ItemInnerSpacing} },         // ImVec2    ItemInnerSpacing
    {"indentSpacing",           {1,ImGuiStyleVar_IndentSpacing} },            // float     IndentSpacing
    {"cellPadding",             {2,ImGuiStyleVar_CellPadding} },              // ImVec2    CellPadding
    {"scrollbarSize",           {1,ImGuiStyleVar_ScrollbarSize} },            // float     ScrollbarSize
    {"scrollbarRounding",       {1,ImGuiStyleVar_ScrollbarRounding} },        // float     ScrollbarRounding
    {"grabMinSize",             {1,ImGuiStyleVar_GrabMinSize} },              // float     GrabMinSize
    {"grabRounding",            {1,ImGuiStyleVar_GrabRounding} },             // float     GrabRounding
    {"tabRounding",             {1,ImGuiStyleVar_TabRounding} },              // float     TabRounding
    {"buttonTextAlign",         {2,ImGuiStyleVar_ButtonTextAlign} },          // ImVec2    ButtonTextAlign
    {"selectableTextAlign",     {2,ImGuiStyleVar_SelectableTextAlign} },      // ImVec2    SelectableTextAlign
    {"separatorTextBorderSize", {1,ImGuiStyleVar_SeparatorTextBorderSize} },  // float  SeparatorTextBorderSize
    {"separatorTextAlign",      {2,ImGuiStyleVar_SeparatorTextAlign} },       // ImVec2    SeparatorTextAlign
    {"separatorTextPadding",    {2,ImGuiStyleVar_SeparatorTextPadding} },     // ImVec2    SeparatorTextPadding
};

std::unordered_map<std::string,ImGuiCol_> map_style_color_name = {
    {"colorText",                   ImGuiCol_Text},
    {"colorTextDisabled",           ImGuiCol_TextDisabled},
    {"colorWindowBg",               ImGuiCol_WindowBg},              // Background of normal windows
    {"colorChildBg",                ImGuiCol_ChildBg},               // Background of child windows
    {"colorPopupBg",                ImGuiCol_PopupBg},               // Background of popups, menus, tooltips windows
    {"colorBorder",                 ImGuiCol_Border},
    {"colorBorderShadow",           ImGuiCol_BorderShadow},
    {"colorFrameBg",                ImGuiCol_FrameBg},               // Background of checkbox, radio button, plot, slider, text input
    {"colorFrameBgHovered",         ImGuiCol_FrameBgHovered},
    {"colorFrameBgActive",          ImGuiCol_FrameBgActive},
    {"colorTitleBg",                ImGuiCol_TitleBg},
    {"colorTitleBgActive",          ImGuiCol_TitleBgActive},
    {"colorTitleBgCollapsed",       ImGuiCol_TitleBgCollapsed},
    {"colorMenuBarBg",              ImGuiCol_MenuBarBg},
    {"colorScrollbarBg",            ImGuiCol_ScrollbarBg},
    {"colorScrollbarGrab",          ImGuiCol_ScrollbarGrab},
    {"colorScrollbarGrabHovered",   ImGuiCol_ScrollbarGrabHovered},
    {"colorScrollbarGrabActive",    ImGuiCol_ScrollbarGrabActive},
    {"colorCheckMark",              ImGuiCol_CheckMark},
    {"colorSliderGrab",             ImGuiCol_SliderGrab},
    {"colorSliderGrabActive",       ImGuiCol_SliderGrabActive},
    {"colorButton",                 ImGuiCol_Button},
    {"colorButtonHovered",          ImGuiCol_ButtonHovered},
    {"colorButtonActive",           ImGuiCol_ButtonActive},
    {"colorHeader",                 ImGuiCol_Header},                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    {"colorHeaderHovered",          ImGuiCol_HeaderHovered},
    {"colorHeaderActive",           ImGuiCol_HeaderActive},
    {"colorSeparator",              ImGuiCol_Separator},
    {"colorSeparatorHovered",       ImGuiCol_SeparatorHovered},
    {"colorSeparatorActive",        ImGuiCol_SeparatorActive},
    {"colorResizeGrip",             ImGuiCol_ResizeGrip},            // Resize grip in lower-right and lower-left corners of windows.
    {"colorResizeGripHovered",      ImGuiCol_ResizeGripHovered},
    {"colorResizeGripActive",       ImGuiCol_ResizeGripActive},
    {"colorTab",                    ImGuiCol_Tab},                   // TabItem in a TabBar
    {"colorTabHovered",             ImGuiCol_TabHovered},
    {"colorTabActive",              ImGuiCol_TabActive},
    {"colorTabUnfocused",           ImGuiCol_TabUnfocused},
    {"colorTabUnfocusedActive",     ImGuiCol_TabUnfocusedActive},
    {"colorPlotLines",              ImGuiCol_PlotLines},
    {"colorPlotLinesHovered",       ImGuiCol_PlotLinesHovered},
    {"colorPlotHistogram",          ImGuiCol_PlotHistogram},
    {"colorPlotHistogramHovered",   ImGuiCol_PlotHistogramHovered},
    {"colorTableHeaderBg",          ImGuiCol_TableHeaderBg},         // Table header background
    {"colorTableBorderStrong",      ImGuiCol_TableBorderStrong},     // Table outer and header borders (prefer using Alpha=1.0 here)
    {"colorTableBorderLight",       ImGuiCol_TableBorderLight},      // Table inner borders (prefer using Alpha=1.0 here)
    {"colorTableRowBg",             ImGuiCol_TableRowBg},            // Table row background (even rows)
    {"colorTableRowBgAlt",          ImGuiCol_TableRowBgAlt},         // Table row background (odd rows)
    {"colorTextSelectedBg",         ImGuiCol_TextSelectedBg},
    {"colorDragDropTarget",         ImGuiCol_DragDropTarget},        // Rectangle highlighting a drop target
    {"colorNavHighlight",           ImGuiCol_NavHighlight},          // Gamepad/keyboard: current highlighted item
    {"colorNavWindowingHighlight",  ImGuiCol_NavWindowingHighlight}, // Highlight window when using CTRL+TAB
    {"colorNavWindowingDimBg",      ImGuiCol_NavWindowingDimBg},     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    {"colorModalWindowDimBg",       ImGuiCol_ModalWindowDimBg}      // Darken/colorize entire screen behind a modal window, when one is active
};

ImGuiYogaRender::ImGuiYogaRender()
{

}

int32_t ImGuiYogaRender::on_render_elements(YogaElement &element)
{
//    MR_INFO("render wiget:{} id:{} pos:{}#{}#{}#{} size:{}x{}",element.widget_,element.id_,
//             element.top_,element.left_,element.bottom_,element.right_,
//             element.width_,element.height_);

    if(!theme_loaded_){
        theme_loaded_ = true;
        load_theme("simpleBlackTheme");
    }
    if(element.widget_.empty()){
        return 0;
    }

    ImVec2 size(element.width_,element.height_);

    if(element.widget_ == "Window"){
        ImGui::Begin(element.id_.c_str());
        return 0;
    }

    //MR_TIMER_NEW(t);
    int32_t pushed_style_color = 0;
    int32_t pushed_style_var = 0;
    element_push_style_var(element,pushed_style_color,pushed_style_var,&default_push_setter_);
    //MR_INFO("push style use {} us",MR_TIMER_US(t));

    ImGui::SetCursorPos(ImVec2(element.left_,element.top_) + ImGui::GetWindowContentRegionMin() );
    if(element.widget_ == "Button" || element.widget_ == "ImageButton"){
        int32_t pushed_count = 0;

        bool clicked = false;
        if(element.widget_ == "Button")
            clicked = ImGui::Button(element.style_value("text","").c_str(),size);
        else
            clicked = mrui::ImageButton(element.id_.c_str(),element.style_value("image","").c_str(),NULL,size);ImGui::SameLine();

        if(pushed_count)
            ImGui::PopStyleColor(pushed_count);

        if(clicked){
            element.emit_event("onclicked");
        }
    }
    else if(element.widget_ == "Image"){
        mrui::Image(element.style_value("image","").c_str(),NULL,size);ImGui::SameLine();
    }
    if(element.widget_ == "Text"){
        ImGui::SetNextItemWidth(element.width_);
        ImGui::Text("%s", element.style_value("text","").c_str());
    }
    else if(element.widget_ == "Slider"){
        static float a = 0;
        ImGui::SetNextItemWidth(element.width_);
        ImGui::SliderFloat(element.id_.c_str(),&a,0,100);
    }

    if(pushed_style_color)
        ImGui::PopStyleColor(pushed_style_color);
    if(pushed_style_var)
        ImGui::PopStyleVar(pushed_style_var);

    return 0;
}

int32_t ImGuiYogaRender::after_render_elements(YogaElement &element)
{
    if(element.widget_ == "Window"){
        ImGui::End();
    }
    else if(element.widget_ == "ChildWindow"){
        ImGui::EndChild();
    }
    return 0;
}

int32_t ImGuiYogaRender::element_push_style_var(YogaElement &element, int32_t& pushed_style_color, int32_t& pushed_style_var, StyleSetter *setter){
    pushed_style_color = 0;
    pushed_style_var = 0;

    for(auto& style : element.styles_){
        const StyleValue& style_value = style.second;

        auto color_it = map_style_color_name.find(style.first);
        if(color_it != map_style_color_name.end() ){
            if(style_value.index() == kStyleValueNumberIndex){
                uint32_t color = std::get<double>(style_value);
                SWAP_ENDIAN(color)
                if(setter && setter->color_setter_uint32){
                    pushed_style_color++;
                    setter->color_setter_uint32(color_it->second,color);
                }
            }
            else if(style_value.index() == kStyleValueNumberArrayIndex){
                auto v = std::get<std::vector<double>>(style_value);
                if(v.empty())
                    continue;
                ImVec4 value(0,0,0,1);
                float* walker = (float*)&value;
                int count = std::min(v.size(),(size_t)4);
                float float_div = 1;
                for(auto item : v){
                    //use int for color values,if need int color{1,1,1,1}, use {1,1,1,1,255}
                    if(item > 1.0001){
                        float_div = 255;break;
                    }
                }
                for(int index = 0;index < count; index++){
                    *walker++ = v[index] / float_div;
                }
                if(setter && setter->color_setter_vec4){
                    pushed_style_color++;
                    setter->color_setter_vec4(color_it->second,value);
                }
            }
            continue;
        }

        auto var_it = map_style_var_name.find(style.first);
        if(var_it != map_style_var_name.end()){
            const ImGuiStyleVarInfo& info = var_it->second;
            if(info.value_count == 1 && style_value.index() == kStyleValueNumberIndex){
                if(setter && setter->var_setter_float){
                    pushed_style_var++;
                    float value = std::get<double>(style_value);
                    setter->var_setter_float(info.type,value);
                }
            }
            else if(info.value_count == 2){
                ImVec2 value(0,0);
                if(style_value.index() == kStyleValueNumberIndex){
                    auto v = std::get<double>(style_value);
                    value.x = value.y = v;
                }
                else if(style_value.index() == kStyleValueNumberArrayIndex){
                    auto v = std::get<std::vector<double>>(style_value);
                    if(v.empty())
                        continue;

                    if(v.size() == 1)
                        value.x = value.y = v[0];
                    else{
                        value.x = v[0];
                        value.y = v[1];
                    }
                }
                if(setter && setter->var_setter_vec2){
                    pushed_style_var++;
                    setter->var_setter_vec2(info.type,value);
                }
            }
        }
    }
    return pushed_style_color + pushed_style_var;
}

int32_t ImGuiYogaRender::load_theme(const std::string &theme_name)
{
    sol::state lua;
    lua.open_libraries();
    std::string theme_file = std::filesystem::path(mr::current_executable_dir()) / "resources" / "theme_default.lua";
    try{
        lua.safe_script_file(theme_file);
        sol::optional<sol::table> theme_opt = lua[theme_name];
        if(theme_opt != sol::nullopt){
            YogaElement fake_theme_elemen(theme_opt.value());
            StyleSetter theme_setter;
            theme_setter.color_setter_uint32 = [](ImGuiCol idx, ImU32 col){
                ImGui::GetStyle().Colors[idx] = ImGui::ColorConvertU32ToFloat4(col);
            };
            theme_setter.color_setter_vec4 = [](ImGuiCol idx, const ImVec4& col) -> void{
                ImGui::GetStyle().Colors[idx] = col;
            };
            theme_setter.var_setter_float = [](ImGuiStyleVar idx, float val){
                const ImGuiDataVarInfo* var_info = ImGui::GetStyleVarInfo(idx);
                if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
                {
                    float* pvar = (float*)var_info->GetVarPtr(&ImGui::GetStyle());
                    *pvar = val;
                }
            };
            theme_setter.var_setter_vec2 = [](ImGuiStyleVar idx, const ImVec2& val){
                const ImGuiDataVarInfo* var_info = ImGui::GetStyleVarInfo(idx);
                if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
                {
                    ImVec2* pvar = (ImVec2*)var_info->GetVarPtr(&ImGui::GetStyle());
                    *pvar = val;
                }
            };

            int32_t pushed_style_color = 0;
            int32_t pushed_style_var = 0;
            element_push_style_var(fake_theme_elemen,pushed_style_color,pushed_style_var,&theme_setter);
        }
    }
    catch(sol::error& error){
        fprintf(stderr,"ImGuiYogaRender, LUA RUN SCRIPT FILE %s ERROR: %s\n\t",theme_file.c_str(),error.what());
        return -1;
    }
    return 0;
}
