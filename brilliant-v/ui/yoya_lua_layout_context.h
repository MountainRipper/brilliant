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
    int32_t set_context_variant(const std::string_view &name, const CompatValue& value);
    int32_t set_context_variant_ref(const std::string_view& name,CompatValue& value);
private:
    std::shared_ptr<sol::state> lua_;
    std::map<std::string,YogaLuaLayout> layouts_origin_;
};

#endif // YOGALUALAYOUTCONTEXT_H
