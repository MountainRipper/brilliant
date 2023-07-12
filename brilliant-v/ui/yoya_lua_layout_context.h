#ifndef YOGALUALAYOUTCONTEXT_H
#define YOGALUALAYOUTCONTEXT_H
#include "yoga_lua_layout.h"
#include <memory>
namespace sol {
class state;
}
class YogaLuaLayoutContext
{
public:
    YogaLuaLayoutContext();
    int32_t load(const std::string& ui_script);
    YogaLuaLayout* get_layout(const std::string& id);
private:
    std::shared_ptr<sol::state> lua_;
};

#endif // YOGALUALAYOUTCONTEXT_H
