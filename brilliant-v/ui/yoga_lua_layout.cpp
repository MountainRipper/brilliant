#include "yoga_lua_layout.h"
#include "yoya_lua_layout_context.h"

#include "yoga/YGNode.h"
#include "yoga/Yoga.h"
#include <sol/sol.hpp>


const std::map<std::string,YGFlexDirection> flex_direction_map = {
    {"column",          YGFlexDirectionColumn},
    {"column-reverse",  YGFlexDirectionColumnReverse},
    {"row",             YGFlexDirectionRow},
    {"row-reverse",     YGFlexDirectionRowReverse}};
const std::map<std::string,YGWrap> wrap_map = {
    {"wrap",            YGWrapWrap},
    {"no-wrap",         YGWrapNoWrap},
    {"wrap-reverse",    YGWrapWrapReverse}};
const std::map<std::string,YGDirection> direction_map = {
    {"inherit",         YGDirectionInherit},
    {"ltr",             YGDirectionLTR},
    {"rtl",             YGDirectionRTL}};
const std::map<std::string,YGJustify> justify_map = {
    {"flex-start",      YGJustifyFlexStart},
    {"center",          YGJustifyCenter},
    {"flex-end",        YGJustifyFlexEnd},
    {"space-between",   YGJustifySpaceBetween},
    {"space-around",    YGJustifySpaceAround},
    {"space-evenly",    YGJustifySpaceEvenly}};
const std::map<std::string,YGAlign> align_map = {
    {"auto",            YGAlignAuto},
    {"flex-start",      YGAlignFlexStart},
    {"center",          YGAlignCenter},
    {"flex-end",        YGAlignFlexEnd},
    {"stretch",         YGAlignStretch},
    {"baseline",        YGAlignBaseline},
    {"space-between",   YGAlignSpaceBetween},
    {"space-around",    YGAlignSpaceAround}};
const std::map<std::string,YGPositionType> position_type_map = {
    {"static",          YGPositionTypeStatic},
    {"relative",        YGPositionTypeRelative},
    {"absolute",        YGPositionTypeAbsolute}};


inline YGValue LUA_YG_VALUE(sol::table &table, std::string_view name){
    YGValue value{0,YGUnitUndefined};
    sol::optional<float> opt = table[name];
    if(opt != sol::nullopt){
        value.value = opt.value();
        value.unit = YGUnitPoint;
    }
    else{
        sol::optional<std::string> opt_str = table[name];
        if(opt_str != sol::nullopt){
            std::string& str = opt_str.value();
            if(str == "auto"){
                value.unit = YGUnitAuto;
            }
            else if(str.size() && str[str.size()-1] == '%'){
                value.unit = YGUnitPercent;
                value.value = atoi(str.c_str());
            }
        }
    }
    return value;
}

ElementPropertyValue lua_get_style_value(sol::lua_value& value){
    ElementPropertyValue property_value;
    switch (value.value().get_type()) {
    case sol::type::number:property_value = value.as<double>();break;
    case sol::type::string:property_value = value.as<std::string>();break;
    case sol::type::boolean:property_value = value.as<bool>();break;
    case sol::type::userdata:
    case sol::type::lightuserdata:property_value = value.as<void*>();break;
    case sol::type::table:{
        auto tb = value.as<sol::table>();
        if(tb.size()){
            auto type = tb[1].get_type();
            if(type == sol::type::number){
                property_value = tb.as<std::vector<double>>();
            }
            else if(type == sol::type::string){
                property_value = tb.as<std::vector<std::string>>();
            }
            else{
                property_value = double(0);
            }
        }
        else
            property_value = double(0);
        break;
    }
    case sol::type::none:
    case sol::type::lua_nil:
    case sol::type::thread:
    case sol::type::function:
    case sol::type::poly:
        break;
    }
    return property_value;
}


template<typename T>
inline T LUA_GET_VALUE(sol::table &table, std::string_view name,T default_value){
    sol::optional<T> opt = table[name];
    if(opt != sol::nullopt){
        return opt.value();
    }
    return default_value;
}

template<typename K,typename T>
inline T MAP_GET_VALUE(const std::map<K,T>& the_map ,const K& key,T default_value) {
    auto it = the_map.find(key);
    if( it != the_map.end())
        return it->second;
    return default_value;
}


#define SET_YG_PROPERTY(node,property,value) \
{ \
        if(value.unit == YGUnitPoint) \
        YGNodeStyleSet##property(node,value.value); \
        else if(value.unit == YGUnitPercent) \
        YGNodeStyleSet##property##Percent(node,value.value); \
        else if(value.unit == YGUnitAuto || value.unit == YGUnitUndefined) \
        YGNodeStyleSet##property##Auto(node); \
}
#define SET_YG_PROPERTY_NO_AUTO(node,property,value) \
{ \
        if(value.unit == YGUnitPoint) \
        YGNodeStyleSet##property(node,value.value); \
        else if(value.unit == YGUnitPercent) \
        YGNodeStyleSet##property##Percent(node,value.value); \
}
#define SET_YG_PROPERTY_EDGE(node,property,edge,value) \
{ \
        if(value.unit == YGUnitPoint || value.unit == YGUnitUndefined) \
        YGNodeStyleSet##property(node,edge,value.value); \
        else if(value.unit == YGUnitPercent) \
        YGNodeStyleSet##property##Percent(node,edge,value.value); \
        else if(value.unit == YGUnitAuto) \
        YGNodeStyleSet##property##Auto(node,edge); \
}
#define SET_YG_PROPERTY_EDGE_NO_AUTO(node,property,edge,value) \
{ \
        if(value.unit == YGUnitPoint) \
        YGNodeStyleSet##property(node,edge,value.value); \
        else if(value.unit == YGUnitPercent) \
        YGNodeStyleSet##property##Percent(node,edge,value.value); \
}
#define GET_LUA_VALUE_AND_SET_YG_PROPERTY(table,key,node,property) \
{ \
        YGValue value{0,YGUnitUndefined}; \
        value = LUA_YG_VALUE(table,key); \
        SET_YG_PROPERTY(node,property,value); \
}
#define GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(table,key,node,property) \
{ \
        YGValue value{0,YGUnitUndefined}; \
        value = LUA_YG_VALUE(table,key); \
        SET_YG_PROPERTY_NO_AUTO(node,property,value); \
}
#define GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(table,key,node,property,edge) \
{ \
        YGValue value{0,YGUnitUndefined}; \
        value = LUA_YG_VALUE(table,key); \
        SET_YG_PROPERTY_EDGE(node,property,edge,value); \
}
#define GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(table,key,node,property,edge) \
{ \
        YGValue value{0,YGUnitUndefined}; \
        value = LUA_YG_VALUE(table,key); \
        SET_YG_PROPERTY_EDGE_NO_AUTO(node,property,edge,value); \
}
#define GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(table,key,node,property,edge) \
{ \
        YGValue value{0,YGUnitUndefined}; \
        value = LUA_YG_VALUE(table,key); \
        if(value.unit == YGUnitPoint) \
        YGNodeStyleSet##property(node,edge,value.value); \
}


YogaElement::YogaElement(sol::table &lua_var)
    :lua_var_(lua_var)
{
    node_ = YGNodeNew();
    parse_node(node_,lua_var);
    parse_widget(lua_var,*this);
}

int32_t YogaElement::push_element(YogaElement &element)
{
    YGNodeInsertChild(node_,element.node_,YGNodeGetChildCount(node_));
    children_.push_back(element);
    return children_.size();
}

int32_t YogaElement::refresh_position(float parent_x, float parent_y)
{
    left_ = YGNodeLayoutGetLeft(node_) + parent_x;
    top_ = YGNodeLayoutGetTop(node_) + parent_y;
    width_ = YGNodeLayoutGetWidth(node_);
    height_ = YGNodeLayoutGetHeight(node_);

    right_ = left_ + width_;
    bottom_ = top_ + height_;
    return 0;
}

int32_t YogaElement::refresh_position_recursion()
{
    float parent_x = 0, parent_y = 0;
    YGNodeRef parent = node_;
    while ((parent = YGNodeGetParent(parent)) != nullptr) {
        parent_x += YGNodeLayoutGetLeft(parent);
        parent_y += YGNodeLayoutGetTop(parent);
    }
    refresh_position(parent_x,parent_y);
    return 0;
}


int32_t YogaElement::parse_widget(sol::table &widget_table, YogaElement &element_self)
{
    sol::optional<std::string> widget_opt = widget_table["widget"];
    if( widget_opt != sol::nullopt ){
        element_self.widget_ = widget_opt.value();
        sol::optional<sol::table> style_opt = widget_table["style"];
        if(style_opt != sol::nullopt){
            sol::table style = style_opt.value();
            for(auto& item : style){
                std::string style_name = item.first.as<std::string>();
                sol::lua_value value = item.second;
                auto style_value = lua_get_style_value(value);
                element_self.properties_[style_name] = style_value;
            }
        }
    }

    sol::optional<std::string> id_opt = widget_table["id"];
    if( id_opt != sol::nullopt ){
        element_self.id_ = id_opt.value();
    }

    return 0;
}



int32_t YogaElement::parse_node(YGNode* node,sol::table &node_define)
{
    //flex properties
    auto flexDirection = LUA_GET_VALUE<std::string>(node_define,"flexDirection","row");
    YGNodeStyleSetFlexDirection(node, MAP_GET_VALUE(flex_direction_map,flexDirection,YGFlexDirectionRow));

    auto flexGrow = LUA_GET_VALUE<float>(node_define,"flexGrow", 0);
    YGNodeStyleSetFlexGrow(node,flexGrow);

    auto flexShrink = LUA_GET_VALUE<float>(node_define,"flexShrink", 1);
    YGNodeStyleSetFlexShrink(node,flexShrink);

    GET_LUA_VALUE_AND_SET_YG_PROPERTY(node_define,"flexBasis",node,FlexBasis)

    auto flexWrap = LUA_GET_VALUE<std::string>(node_define,"flexWrap","no-wrap");
    YGNodeStyleSetFlexWrap(node, MAP_GET_VALUE(wrap_map,flexWrap,YGWrapNoWrap));

    auto direction = LUA_GET_VALUE<std::string>(node_define,"direction","inherit");
    YGNodeStyleSetDirection(node, MAP_GET_VALUE(direction_map,direction,YGDirectionInherit));

    //alignment properties
    auto justifyContent = LUA_GET_VALUE<std::string>(node_define,"justifyContent","flex-start");
    YGNodeStyleSetJustifyContent(node, MAP_GET_VALUE(justify_map,justifyContent,YGJustifyFlexStart));

    auto alignContents = LUA_GET_VALUE<std::string>(node_define,"alignContents","stretch");
    YGNodeStyleSetAlignContent(node, MAP_GET_VALUE(align_map,alignContents,YGAlignAuto));

    auto alignItems = LUA_GET_VALUE<std::string>(node_define,"alignItems","stretch");
    YGNodeStyleSetAlignItems(node, MAP_GET_VALUE(align_map,alignItems,YGAlignAuto));

    auto alignSelf = LUA_GET_VALUE<std::string>(node_define,"alignSelf","auto");
    YGNodeStyleSetAlignSelf(node, MAP_GET_VALUE(align_map,alignSelf,YGAlignAuto));

    //position properties
    auto aspectRatio = LUA_GET_VALUE<float>(node_define,"aspectRatio", 0);
    if(aspectRatio > 0) YGNodeStyleSetAspectRatio(node,aspectRatio);

    auto positionType = LUA_GET_VALUE<std::string>(node_define,"positionType","relative");
    YGNodeStyleSetPositionType(node,MAP_GET_VALUE(position_type_map,positionType,YGPositionTypeRelative));


    GET_LUA_VALUE_AND_SET_YG_PROPERTY(node_define,"width", node,Width)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY(node_define,"height",node,Height)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(node_define,"minimumWidth", node,MinWidth)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(node_define,"minimumHeight",node,MinHeight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(node_define,"maximumWidth", node,MaxWidth)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(node_define,"maximumHeight",node,MaxHeight)


    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(node_define,"marginAll"      ,node,Margin,YGEdgeAll)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(node_define,"marginBottom"   ,node,Margin,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(node_define,"marginLeft"     ,node,Margin,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(node_define,"marginRight"    ,node,Margin,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(node_define,"marginTop"      ,node,Margin,YGEdgeTop)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(node_define,"paddingAll"     ,node,Padding,YGEdgeAll)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(node_define,"paddingBottom"  ,node,Padding,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(node_define,"paddingLeft"    ,node,Padding,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(node_define,"paddingRight"   ,node,Padding,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(node_define,"paddingTop"     ,node,Padding,YGEdgeTop)

    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"borderAll"      ,node,Border,YGEdgeAll)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"borderBottom"   ,node,Border,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"borderLeft"     ,node,Border,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"borderRight"    ,node,Border,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"borderTop"      ,node,Border,YGEdgeTop)

    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"positionBottom" ,node,Position,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"positionLeft"   ,node,Position,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"positionRight"  ,node,Position,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(node_define,"positionTop"    ,node,Position,YGEdgeTop)

    return 0;
}


YogaLuaLayout::YogaLuaLayout(sol::table &lua_var)
    :YogaElement(lua_var)
{


}

int32_t YogaLuaLayout::lua_set_element_property(const std::string &id,const  std::string &property, sol::lua_value value)
{
    return 0;
}



YogaLuaLayout *YogaLuaLayout::clone(sol::table &self)
{
    return nullptr;
}

int32_t YogaLuaLayout::set_renderer(YogaLuaLayoutRenderer *renderer)
{
    renderer_ = renderer;
    return 0;
}

int32_t YogaLuaLayout::render_frame()
{
    if(!renderer_)
        return -1;

    std::function<void(YogaElement& element)> renderer_caller;
    renderer_caller = [&renderer_caller,this](YogaElement& element)->void{
        renderer_->on_render_elements(element);
        for(auto& item : element.children_){
            renderer_caller(item);
        }
        renderer_->after_render_elements(element);
    };

    renderer_caller(*this);
    return 0;
}
