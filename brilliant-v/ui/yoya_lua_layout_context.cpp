#include "yoya_lua_layout_context.h"
#include <sol/sol.hpp>
#include <mrcommon/filesystem.h>
#include <filesystem>
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
        auto script = R"(package.path=libpath..";"..package.path;)";
        lua_->safe_script(script);
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
    return 0;
}
