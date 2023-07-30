#include <sstream>
#include <sol/sol.hpp>
#include <mrcommon/filesystem.h>
#include <mrcommon/logger.h>
#include <filesystem>
#include <yoga/Yoga.h>
#include <yoga/YGNode.h>
#include "yoya_lua_layout_context.h"

auto init_script = R"(
package.path=libpath..";"..package.path;

oo = require('bhou.oo.base')

globalWidgets={}

function classOf(Object)
    return oo.new(Object)
end

function registerWidgets(name,widget)
    globalWidgets[name] = widget
end

function createWidget(name)
    if(globalWidgets[name] ~= nil) then
        return oo.new(globalWidgets[name])
    end
    return nil
end
)";

#include "yoga_imgui_widget.h"

YogaLuaLayoutContext::YogaLuaLayoutContext()
{

}

int32_t YogaLuaLayoutContext::load(const std::string &ui_script, int32_t screen_width, int32_t screen_height)
{
    if(!lua_){
        lua_ = std::shared_ptr<sol::state>(new sol::state());
        lua_->open_libraries();
        std::string cur_dir = mr::current_executable_dir();
        (*lua_)["libpath"] = (std::filesystem::path(cur_dir) / "script" / "lib"/ "?.lua" ).string();
        (*lua_)["nativeSetElementProperty"] = &YogaLuaLayout::lua_set_element_property;
        (*lua_)["screenWidth"] = screen_width;
        (*lua_)["screenHeight"] = screen_height;
        lua_->new_usertype<YogaElement>( "YogaElement",
                                SOL_PPROPERTY(YogaElement,number),
                                SOL_PPROPERTY(YogaElement,boolean),
                                SOL_PPROPERTY(YogaElement,string),
                                SOL_PPROPERTY(YogaElement,number_array),
                                SOL_PPROPERTY(YogaElement,string_array),
                                SOL_PPROPERTY(YogaElement,pointer)
                                );


        lua_->safe_script(init_script);
    }
    try{
        lua_->safe_script_file(ui_script);
    }
    catch(sol::error& error){
        fprintf(stderr,"LUA RUN SCRIPT FILE ERROR: %s\n\t",error.what());
        return -1;
    }

    return 0;
}

YogaLuaLayout *YogaLuaLayoutContext::get_layout(const std::string &id)
{
    MR_TIMER_NEW(t);
    YogaLuaLayout* layout = nullptr;
    if(layouts_origin_.find(id) == layouts_origin_.end()){
        sol::protected_function creator = (*lua_)["createWidget"];
        if(creator){
            sol::optional<sol::table> object = creator(id);
            if(object != sol::nullopt){
                sol::optional<sol::table> ui_table = object.value()["ui"];
                if(ui_table != sol::nullopt){
                    //current use ui in owner object
                    //TODO: if object(logic) and ui define separated is need
                    layout = new YogaLuaLayout(object.value(),ui_table.value());
                }
                object.value()["parseNamedElements"](object.value());
            }
        }
    }
    if(layout == nullptr)
        return nullptr;

    layout->refresh_position_all();
    layout->refresh_named_elements();

    MR_INFO("parse and calc use {} ms",MR_TIMER_MS(t));
    return layout;
}

int32_t YogaLuaLayoutContext::set_context_variant(const std::string_view &name,const CompatValue &value)
{
    auto type_index = value.index();
    switch (type_index) {
    case kCompatValueNumberIndex: {
        (*lua_)[name] = (double)value;
        break;
    }
    case kCompatValueBooleanIndex: {
        (*lua_)[name] = (bool)value;
        break;
    }
    case kCompatValueStringIndex: {
        (*lua_)[name] = (std::string)value;
        break;
    }
    case kCompatValueNumberArrayIndex: {
        (*lua_)[name] = (std::vector<double>)value;
        break;
    }
    case kCompatValueStringArrayIndex: {
        (*lua_)[name] = (std::vector<std::string>)value;
        break;
    }
    case kCompatValuePointerIndex: {
        (*lua_)[name] = (void*)value;
        break;
    }
    }
    return 0;
}

int32_t YogaLuaLayoutContext::set_context_variant_ref(const std::string_view &name, CompatValue &value)
{
    auto type_index = value.index();
    switch (type_index) {
    case kCompatValueNumberIndex: {
        (*lua_)[name] = value.ref_number();
        break;
    }
    case kCompatValueBooleanIndex: {
        (*lua_)[name] = value.ref_boolean();
        break;
    }
    case kCompatValueStringIndex: {
        (*lua_)[name] = value.ref_string();
        break;
    }
    case kCompatValueNumberArrayIndex: {
        (*lua_)[name] = value.ref_number_array();
        break;
    }
    case kCompatValueStringArrayIndex: {
        (*lua_)[name] = value.ref_string_array();
        break;
    }
    case kCompatValuePointerIndex: {
        (*lua_)[name] = value.ref_pointer();
        break;
    }
    }
    return 0;
}
