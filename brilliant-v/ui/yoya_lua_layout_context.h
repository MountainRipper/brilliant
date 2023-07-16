#ifndef YOGALUALAYOUTCONTEXT_H
#define YOGALUALAYOUTCONTEXT_H
#include <memory>
#include <map>
#include <sol/forward.hpp>
#include "yoga_lua_layout.h"

class YogaLuaLayoutContext
{
public:
    YogaLuaLayoutContext();
    int32_t load(const std::string& ui_script);
    YogaLuaLayout* get_layout(const std::string& id);
private:
    std::shared_ptr<sol::state> lua_;
    std::map<std::string,YogaLuaLayout> layouts_origin_;
};

#endif // YOGALUALAYOUTCONTEXT_H
