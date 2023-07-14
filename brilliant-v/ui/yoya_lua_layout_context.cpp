#include <sstream>
#include <sol/sol.hpp>
#include <mrcommon/filesystem.h>
#include <filesystem>
#include <yoga/Yoga.h>
#include <yoga/YGNode.h>
#include "yoya_lua_layout_context.h"

auto init_script = R"(
package.path=libpath..";"..package.path;
globalWidgets={}
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


YogaLuaLayoutContext::YogaLuaLayoutContext()
{

}

int32_t YogaLuaLayoutContext::load(const std::string &ui_script)
{
    if(!lua_){
        lua_ = std::shared_ptr<sol::state>(new sol::state());
        lua_->open_libraries();
        std::string cur_dir = mr::current_executable_dir();
        (*lua_)["libpath"] = (std::filesystem::path(cur_dir) / "script" / "lib"/ "?.lua" ).string();
        (*lua_)["nativeSetElementProperty"] = &YogaLuaLayout::lua_set_element_property;
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
    YogaLuaLayout* layout = nullptr;
    if(layouts_origin_.find(id) == layouts_origin_.end()){
        sol::protected_function creator = (*lua_)["createWidget"];
        if(creator){
            sol::optional<sol::table> object = creator(id);
            if(object != sol::nullopt){
                sol::optional<sol::table> layout_table = object.value()["layout"];
                if(layout_table != sol::nullopt){
                    layout = new YogaLuaLayout(layout_table.value());
                    parse_layouts(layout_table.value(),*layout);
                }
            }
        }
    }

    YGNodeCalculateLayout(layout->node_, 1000, 1000, YGDirectionLTR);

    std::function<void(YogaElement& element,float parent_x,float parent_y)> refresher;

    refresher = [&refresher](YogaElement& element,float parent_x,float parent_y)->void{
        element.refresh_position(parent_x,parent_y);
        for(auto& item : element.children_){
            refresher(item,element.left_,element.top_);
        }
    };

    refresher(*layout,0,0);

    return layout;
}

int32_t YogaLuaLayoutContext::parse_layouts(sol::table &widget_table,YogaElement& element_self)
{
    sol::optional<sol::table> elements = widget_table["elements"];
    if(elements != sol::nullopt){

        for(const auto &it : elements.value()){
            sol::lua_value element = it.second;
            auto type = element.value().get_type();
            if(type ==sol::type::table){
                sol::table element_table = element.as<sol::table>();
                YogaElement element_clild(element_table);
                parse_layouts(element_table,element_clild);

                element_self.push_element(element_clild);
            }
        }
    }


    return 0;
}
