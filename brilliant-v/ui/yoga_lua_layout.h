#ifndef YOGALUALAYOUT_H
#define YOGALUALAYOUT_H
#include <string>
#include <map>
#include <variant>
#include <vector>
#include <sol/forward.hpp>
class YGNode;

typedef std::variant<double,bool,std::string,std::vector<double>,std::vector<std::string>,void*> ElementPropertyValue;

class YogaElement{
public:
    YogaElement(sol::table& lua_var);
    int32_t push_element(YogaElement& element);
    int32_t refresh_position(float parent_x = 0, float parent_y = 0);
    int32_t refresh_position_recursion();
public:
    static int32_t parse_node(YGNode *node, sol::table& node_define);
    static int32_t parse_widget(sol::table& widget_table, YogaElement &element_self);

public:
    YGNode* node_   = 0;
    std::vector<YogaElement> children_;

    std::map<std::string,ElementPropertyValue> properties_;
    std::string widget_;
    std::string id_;
    float left_     = 0;
    float right_    = 0;
    float top_      = 0;
    float bottom_   = 0;
    float width_    = 0;
    float height_   = 0;
    sol::table& lua_var_;
};

class YogaLuaLayoutRenderer{
public:
    virtual int32_t on_render_elements(YogaElement& element) = 0;
    virtual int32_t after_render_elements(YogaElement &element) = 0;
};

class YogaLuaLayout : public YogaElement
{
public:
    YogaLuaLayout(sol::table &lua_var);

    YogaLuaLayout* clone(sol::table& self);
    int32_t set_renderer(YogaLuaLayoutRenderer* renderer);

    int32_t render_frame();
    int32_t lua_set_element_property(const std::string &id, const std::string &property, sol::lua_value value);

private:
    YogaLuaLayoutRenderer* renderer_ = nullptr;
};

#endif // YOGALUALAYOUT_H
