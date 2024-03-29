
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <filesystem>
#include <mrcommon/logger.h>
#include <mrcommon/filesystem.h>
#include <sol/sol.hpp>
#include "yoga_imgui_render.h"
#include "imgui_widget_mr.h"

const ImGuiID kStateIdDragScrolling = 100001;

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

int32_t ImGuiYogaRender::attach_environment(sol::state &lua_state)
{
    //lua_state.new_usertype<mrui::ListView::AbstractModel>( "ListModel");
}

int32_t ImGuiYogaRender::on_render_elements(YogaElement &element)
{
//    MR_INFO("render wiget:{} id:{} pos:{}#{}#{}#{} size:{}x{}",element.widget_,element.id_,
//             element.top_,element.left_,element.bottom_,element.right_,
//             element.width_,element.height_);

    if(!theme_loaded_){
        theme_loaded_ = true;
        load_theme("defaultTheme");
    }
    if(element.widget_.empty()){
        return 0;
    }

    ImVec2 size = element.size<ImVec2>();

    if(element.widget_ == "Window"){
        //ImGui::SetNextWindowSize(size);
        ImGui::Begin(element.id_.c_str());
        return 0;
    }
    if(element.widget_ == "ChildWindow"){
        ImGui::BeginChild("__",ImVec2(1000,1000));
        return 0;
    }

    //MR_TIMER_NEW(t);
    int32_t pushed_style_color = 0;
    int32_t pushed_style_var = 0;
    element_push_style_var(element,pushed_style_color,pushed_style_var,&default_push_setter_);
    //MR_INFO("push style use {} us",MR_TIMER_US(t));

    ImVec2 position = element.left_top<ImVec2>();
    ImGui::SetCursorPos(position + ImGui::GetWindowContentRegionMin() + ImGui::GetCurrentWindow()->Scroll) ;

    ImGui::PushID(&element);

    bool value_changed = false;
    if(element.widget_ == "Button" || element.widget_ == "ImageButton" ||  element.widget_ == "RoundImageButton"){


        uint32_t tint_color = element.style_color("colorTint",0xFFFFFFFF);
        bool clicked = false;
        if(element.widget_ == "Button")
            clicked = ImGui::Button(element.style_value("text","").c_str(),size);
        else if(element.widget_ == "ImageButton"){
            clicked = mrui::ImageButton(element.id_.c_str(),element.style_value("image","").c_str(),NULL,size,ImVec4(0,0,0,0),ImGui::ColorConvertU32ToFloat4(tint_color));
        }
        else if(element.widget_ == "RoundImageButton"){
            float radius = element.style_value("frameRounding",double(0),nullptr);
            clicked = mrui::RoundImageButton(element.id_.c_str(),element.style_value("image","").c_str(),NULL,size,ImGui::ColorConvertU32ToFloat4(tint_color),radius);
        }

        bool drag_scrolling = ImGui::GetCurrentWindow()->StateStorage.GetBool(kStateIdDragScrolling);
        if(clicked && !drag_scrolling){
            element.emit_event("onclicked");
        }
    }
    else if(element.widget_ == "Image"){
        bool exist = false;
        float radius = element.style_value("frameRounding",double(0),&exist);
        uint32_t color = element.style_color("color",0xFFFFFFFF);
        if(radius > 0){
            mrui::ImageRound(element.style_value("image","").c_str(),NULL,size,radius,ImGui::ColorConvertU32ToFloat4(color));
        }
        else{
            mrui::Image(element.style_value("image","").c_str(),NULL,size,ImGui::ColorConvertU32ToFloat4(color));
        }
    }
    if(element.widget_ == "Text"){
        //ImGui::SetNextItemWidth(element.width_);
        //ImGui::Text("%s", element.style_value("text","").c_str());
        std::string aligin_h = element.style_value("aliginH",std::string("left"));
        std::string aligin_v = element.style_value("aliginV",std::string("top"));
        int i_aligin_h = 0;
        if(aligin_h == "center")
            i_aligin_h = 1;
        else if(aligin_h == "right")
            i_aligin_h = 2;

        int i_aligin_v = 0;
        if(aligin_v == "center")
            i_aligin_v = 1;
        else if(aligin_v == "bottom")
            i_aligin_v = 2;

        bool exist = false;
        uint32_t color = element.style_color("color",0xBBBBBBFF,&exist);
        if(!exist){
            color = ImGui::ColorConvertFloat4ToU32( ImGui::GetStyle().Colors[ImGuiCol_Text] );
        }
        mrui::TextAligined(element.style_value("text","").c_str(),size,i_aligin_h,i_aligin_v,color);
    }
    else if(element.widget_ == "InputText" || element.widget_ == "InputTextMultiline"){
        if(kCompatValueStringIndex != element.value_.index()){
            element.value_ = std::string();
        }
        std::string &value = element.value_.ref_string();
        std::string lable = element.style_value("lable","");
        if(element.widget_ == "InputText"){
            ImGui::SetNextItemWidth(element.width_);
            value_changed = ImGui::InputText(lable.c_str(),&value);
        }
        else
            value_changed = ImGui::InputTextMultiline(lable.c_str(),&value,size);

        if(value_changed)MR_INFO(">>>:{}",value);
    }
    else if(element.widget_ == "Slider"){
        float value_temp = (double)element.value_;
        float value_min = element.style_value("valueMin",double(0));
        float value_max = element.style_value("valueMax",double(1));
        std::string format = element.style_value("valueFormat","%.2f");
        ImGui::SetNextItemWidth(element.width_);
        value_changed = ImGui::SliderFloat(element.id_.c_str(),&value_temp,value_min,value_max,format.c_str());
        element.value_ = value_temp;
    }
    else if(element.widget_ == "RectangleFrame" || element.widget_ == "Rectangle"){
        bool exist = false;
        float radius = element.style_value("frameRounding",double(0),&exist);
        uint32_t color = element.style_color("color");
        if(element.widget_ == "RectangleFrame"){
            float frame_width = element.style_value("frameWidth",double(1),&exist);
            mrui::RectangleFrame(size,color,radius,frame_width);
        }
        else{
            mrui::Rectangle(size,color,radius);
        }
    }
    else if(element.widget_ == "CircelFrame" || element.widget_ == "Circel"){
        bool exist = false;
        float radius = element.style_value("frameRounding",double(0),&exist);
        uint32_t color = element.style_color("color");
        ImVec2 center = ImGui::GetCursorPos() + ImGui::GetCurrentWindow()->Pos + (size / 2) - ImGui::GetCurrentWindow()->Scroll;
        if(element.widget_ == "CircelFrame"){
            float frame_width = element.style_value("frameWidth",double(1),&exist);
            mrui::CircelFrame(center,radius,color,frame_width);
        }
        else{
            mrui::Circel(center,radius,color);
        }
    }
    else if(element.widget_ == "ListView"){
        bool exist = false;
        void* pointer = element.style_value("model",(void*)(nullptr),&exist);
        mrui::ListView::AbstractModel* model = static_cast<mrui::ListView::AbstractModel*>(pointer);

        if(listviews_.find(&element) == listviews_.end()){
            auto new_view = std::make_shared<mrui::ListView>();
            listviews_[&element] = new_view;
        }
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(8,8));
        float item_height = element.style_value("itemHeight",20,&exist);
        std::shared_ptr<mrui::ListView> list_view = listviews_[&element];
        list_view->draw(model,[](mrui::ListView& view,mrui::ListView::AbstractModel* model,int index,int x,int y,int width,int height) ->int32_t{
                ImGui::SetCursorPos(ImVec2(x+8,y));
                mrui::Rectangle(ImVec2(width-16,height-1),0xFF6600FF,8);
                std::string name = model->value(index,"name");
                ImGui::Text("%s",name.c_str());
                return  height;
            },
            element.width_,
            element.height_,
            item_height,
            0);
        ImGui::PopStyleVar();
    }


    if(pushed_style_color)
        ImGui::PopStyleColor(pushed_style_color);
    if(pushed_style_var)
        ImGui::PopStyleVar(pushed_style_var);

    ImGui::PopID();

    if(value_changed)
        element.emit_event("valueChanged");

    return 0;
}

int32_t ImGuiYogaRender::after_render_elements(YogaElement &element)
{
    if(element.widget_ == "Window"){
        ImGui::End();
    }
    else if(element.widget_ == "ChildWindow"){
        bool drag_scrolling = false;
        bool drag_scroll_x = element.style_value("dragScrollX",false);
        bool drag_scroll_y = element.style_value("dragScrollY",false);
        if(drag_scroll_x || drag_scroll_y)
            mrui::DragScrollCurrentWindow(drag_scrolling,ImGuiMouseButton_Left,1,0.9,drag_scroll_y,drag_scroll_x);
        ImGui::GetCurrentWindow()->StateStorage.SetBool(kStateIdDragScrolling,drag_scrolling);
        ImGui::EndChild();
    }
    return 0;
}

int32_t ImGuiYogaRender::element_push_style_var(YogaElement &element, int32_t& pushed_style_color, int32_t& pushed_style_var, StyleSetter *setter){
    pushed_style_color = 0;
    pushed_style_var = 0;

    for(auto& style : element.styles_){
        const CompatValue& style_value = style.second;

        auto color_it = map_style_color_name.find(style.first);
        if(color_it != map_style_color_name.end() ){

            uint32_t color = element.style_value_color(style_value);
            if(setter && setter->color_setter_uint32){
                pushed_style_color++;
                setter->color_setter_uint32(color_it->second,color);
            }
            continue;
        }

        auto var_it = map_style_var_name.find(style.first);
        if(var_it != map_style_var_name.end()){
            const ImGuiStyleVarInfo& info = var_it->second;
            if(info.value_count == 1 && style_value.index() == kCompatValueNumberIndex){
                if(setter && setter->var_setter_float){
                    pushed_style_var++;
                    double value = style_value;
                    setter->var_setter_float(info.type,value);
                }
            }
            else if(info.value_count == 2){
                ImVec2 value(0,0);
                if(style_value.index() == kCompatValueNumberIndex){
                    double v = style_value;
                    value.x = value.y = v;
                }
                else if(style_value.index() == kCompatValueNumberArrayIndex){
                    std::vector<double> v = style_value;
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
    std::string theme_file = (std::filesystem::path(mr::current_executable_dir()) / "resources" / "theme_default.lua").string();
    try{
        lua.safe_script_file(theme_file);
        sol::optional<sol::table> theme_opt = lua[theme_name];
        if(theme_opt != sol::nullopt){
            YogaElement fake_theme_elemen(theme_opt.value(),nullptr);
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
