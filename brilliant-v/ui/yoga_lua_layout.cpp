#include "yoga_lua_layout.h"
#include "yoya_lua_layout_context.h"

#include "yoga/YGNode.h"
#include "yoga/Yoga.h"
#include <sol/sol.hpp>
#include <mrcommon/logger.h>

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

CompatValueType lua_get_style_value(sol::lua_value& value){
    CompatValueType property_value;
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


YogaElement::YogaElement(sol::table& lua_var)
    :lua_var_(lua_var)
{
    node_ = YGNodeNew();
    parse_self_class();
    parse_self_layout();
    parse_self_widget();
    lua_var_["nativeContext"] = this;
}

int32_t YogaElement::push_element(std::shared_ptr<YogaElement> element)
{
    YGNodeInsertChild(node_,element->node_,YGNodeGetChildCount(node_));
    children_.push_back(element);
    return children_.size();
}

int32_t YogaElement::parse_self_class()
{
    sol::optional<sol::table> class_opt = lua_var_["class"];
    if(class_opt == sol::nullopt)
        return 0;

    sol::optional<sol::table> ui_opt = class_opt.value()["ui"];
    if(ui_opt == sol::nullopt)
        return 0;
    sol::table ui = ui_opt.value();
    for(const auto &item : ui){
        std::string style_name = item.first.as<std::string>();
        auto item_value = lua_var_[style_name];
        if(item_value.valid() ){
            continue;
        }
        lua_var_[style_name] = item.second;
    }
    return 0;
}

int32_t YogaElement::parse_self_layout()
{
    parse_node(lua_var_,node_);
    return 0;
}

int32_t YogaElement::parse_self_widget()
{
    parse_widget(lua_var_,*this);
    return 0;
}

int32_t YogaElement::get_back_position(float parent_x, float parent_y)
{
    left_ = YGNodeLayoutGetLeft(node_) + parent_x;
    top_ = YGNodeLayoutGetTop(node_) + parent_y;
    width_ = YGNodeLayoutGetWidth(node_);
    height_ = YGNodeLayoutGetHeight(node_);
    return 0;
}

int32_t YogaElement::get_back_position_recursion()
{
    float parent_x = 0, parent_y = 0;
    YGNodeRef parent = node_;
    while ((parent = YGNodeGetParent(parent)) != nullptr) {
        parent_x += YGNodeLayoutGetLeft(parent);
        parent_y += YGNodeLayoutGetTop(parent);
    }
    get_back_position(parent_x,parent_y);
    return 0;
}

void YogaElement::emit_event(const std::string &event)
{
    sol::optional<sol::function> func = lua_var_[event];
    if(func != sol::nullopt){
        func.value()(lua_var_);
    }
}

uint32_t YogaElement::style_color(const std::string &style, uint32_t defalt_value, bool *exist)
{
    if(exist)
        *exist = false;
    auto color_it = styles_.find(style);
    if(color_it != styles_.end() ){
        auto& style_value = color_it->second;
        if(exist)
            *exist = true;
        return style_value_color(style_value,defalt_value);
    }
    return defalt_value;
}

uint32_t YogaElement::style_value_color(const CompatValue &value,uint32_t defalt_value)
{
    if(value.index() == kCompatValueNumberIndex){
        uint32_t color = value;
        UINT32_SWAP_ENDIAN(color)
        return color;
    }
    else if(value.index() == kCompatValueNumberArrayIndex){
        uint32_t color = 0xFF000000;
        std::vector<double> v = value;
        if(v.empty())
            return color;

        uint8_t* walker = (uint8_t*)&color;
        float float_multiply = 255;
        for(auto item : v){
            //use int for color values,if need int color{1,1,1,1}, use {1,1,1,1,255}
            if(item > 1.0001){
                float_multiply = 1;break;
            }
        }
        int count = std::min(v.size(),(size_t)4);
        for(int index = 0;index < count; index++){
            *walker++ = v[index] * float_multiply;
        }
        return color;
    }
    return defalt_value;
}


int32_t YogaElement::parse_widget(sol::table &element_table, YogaElement &element_self)
{
    sol::optional<std::string> widget_opt = element_table["widget"];
    if( widget_opt != sol::nullopt ){
        element_self.widget_ = widget_opt.value();
        sol::optional<sol::table> style_opt = element_table["style"];
        if(style_opt != sol::nullopt){
            sol::table style = style_opt.value();
            for(auto& item : style){
                std::string style_name = item.first.as<std::string>();
                sol::lua_value value = item.second;
                auto style_value = lua_get_style_value(value);
                element_self.styles_[style_name] = style_value;
            }
        }
    }

    sol::optional<std::string> id_opt = element_table["id"];
    if( id_opt != sol::nullopt ){
        element_self.id_ = id_opt.value();
    }

    if(element_self.widget_ == "TextInput"){

    }
    return 0;
}



int32_t YogaElement::parse_node(sol::table &element_table, YGNode* node)
{
    //flex properties
    auto flexDirection = LUA_GET_VALUE<std::string>(element_table,"flexDirection","row");
    YGNodeStyleSetFlexDirection(node, MAP_GET_VALUE(flex_direction_map,flexDirection,YGFlexDirectionRow));

    auto flexGrow = LUA_GET_VALUE<float>(element_table,"flexGrow", 0);
    YGNodeStyleSetFlexGrow(node,flexGrow);

    auto flexShrink = LUA_GET_VALUE<float>(element_table,"flexShrink", 1);
    YGNodeStyleSetFlexShrink(node,flexShrink);

    GET_LUA_VALUE_AND_SET_YG_PROPERTY(element_table,"flexBasis",node,FlexBasis)

    auto flexWrap = LUA_GET_VALUE<std::string>(element_table,"flexWrap","no-wrap");
    YGNodeStyleSetFlexWrap(node, MAP_GET_VALUE(wrap_map,flexWrap,YGWrapNoWrap));

    auto direction = LUA_GET_VALUE<std::string>(element_table,"direction","inherit");
    YGNodeStyleSetDirection(node, MAP_GET_VALUE(direction_map,direction,YGDirectionInherit));

    //alignment properties
    auto justifyContent = LUA_GET_VALUE<std::string>(element_table,"justifyContent","flex-start");
    YGNodeStyleSetJustifyContent(node, MAP_GET_VALUE(justify_map,justifyContent,YGJustifyFlexStart));

    auto alignContents = LUA_GET_VALUE<std::string>(element_table,"alignContents","stretch");
    YGNodeStyleSetAlignContent(node, MAP_GET_VALUE(align_map,alignContents,YGAlignAuto));

    auto alignItems = LUA_GET_VALUE<std::string>(element_table,"alignItems","stretch");
    YGNodeStyleSetAlignItems(node, MAP_GET_VALUE(align_map,alignItems,YGAlignAuto));

    auto alignSelf = LUA_GET_VALUE<std::string>(element_table,"alignSelf","auto");
    YGNodeStyleSetAlignSelf(node, MAP_GET_VALUE(align_map,alignSelf,YGAlignAuto));

    //position properties
    auto aspectRatio = LUA_GET_VALUE<float>(element_table,"aspectRatio", 0);
    if(aspectRatio > 0) YGNodeStyleSetAspectRatio(node,aspectRatio);

    auto positionType = LUA_GET_VALUE<std::string>(element_table,"positionType","relative");
    YGNodeStyleSetPositionType(node,MAP_GET_VALUE(position_type_map,positionType,YGPositionTypeRelative));


    GET_LUA_VALUE_AND_SET_YG_PROPERTY(element_table,"width", node,Width)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY(element_table,"height",node,Height)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(element_table,"minimumWidth", node,MinWidth)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(element_table,"minimumHeight",node,MinHeight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(element_table,"maximumWidth", node,MaxWidth)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_NO_AUTO(element_table,"maximumHeight",node,MaxHeight)


    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(element_table,"marginAll"      ,node,Margin,YGEdgeAll)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(element_table,"marginBottom"   ,node,Margin,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(element_table,"marginLeft"     ,node,Margin,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(element_table,"marginRight"    ,node,Margin,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE(element_table,"marginTop"      ,node,Margin,YGEdgeTop)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(element_table,"paddingAll"     ,node,Padding,YGEdgeAll)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(element_table,"paddingBottom"  ,node,Padding,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(element_table,"paddingLeft"    ,node,Padding,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(element_table,"paddingRight"   ,node,Padding,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_NO_AUTO(element_table,"paddingTop"     ,node,Padding,YGEdgeTop)

    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"borderAll"      ,node,Border,YGEdgeAll)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"borderBottom"   ,node,Border,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"borderLeft"     ,node,Border,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"borderRight"    ,node,Border,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"borderTop"      ,node,Border,YGEdgeTop)

    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"positionBottom" ,node,Position,YGEdgeBottom)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"positionLeft"   ,node,Position,YGEdgeLeft)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"positionRight"  ,node,Position,YGEdgeRight)
    GET_LUA_VALUE_AND_SET_YG_PROPERTY_EDGE_ONLY_VALUE(element_table,"positionTop"    ,node,Position,YGEdgeTop)

    return 0;
}


YogaLuaLayout::YogaLuaLayout(sol::table &lua_object,sol::table &lua_ui)
    :YogaElement(lua_ui)
    ,object_(lua_object)
{
    auto a = lua_object["ui"].get<sol::table>();
    object_["nativeContext"] = this;
    object_["nativeDoOperator"] = &YogaLuaLayout::lua_do_operator;
    object_["nativeSetElementProperty"] = &YogaLuaLayout::lua_set_element_property;
    parse_child_element_recursion();
}

int32_t YogaLuaLayout::lua_set_element_property(const std::string &id,const  std::string &property, sol::lua_value value)
{
    return 0;
}

int32_t YogaLuaLayout::lua_do_operator(const std::string &method, sol::lua_value value)
{
    if(method == "dirty"){
        std::string id = value.as<std::string>();
        std::map<std::string,YogaElement*>::iterator it = named_elements_.find(id);
        if( it != named_elements_.end()){
            YogaElement* element = it->second;
            //MR_TIMER_NEW(t);
            element->parse_self_layout();
            //MR_INFO("dirty parse property use {} ms",MR_TIMER_MS(t));
        }
        dirty_ = true;
    }
    return 0;
}

int32_t YogaLuaLayout::parse_child_element_recursion()
{
    //current is static layout
    //TODO:when dynamic create element, whether need clear all and re create elements???
    children_.clear();

    std::function<int32_t(sol::table &,YogaElement&)> element_parser;
    element_parser = [&element_parser](sol::table &widget_table,YogaElement& element_self) ->int32_t{
        sol::optional<sol::table> elements = widget_table["elements"];
        if(elements != sol::nullopt){

            for(const auto &it : elements.value()){
                sol::lua_value element = it.second;
                auto type = element.value().get_type();
                if(type ==sol::type::table){
                    sol::table element_table = element.as<sol::table>();
                    auto element_child = std::make_shared<YogaElement>(element_table);
                    element_parser(element_table,*element_child);

                    element_self.push_element(element_child);
                }
            }
        }
        return 0;
    };
    element_parser(lua_var_,*this);
    return 0;
}

int32_t YogaLuaLayout::foreach_elements(ElementOperator visitor)
{
    std::function<void(YogaElement& element,ElementOperator& visitor)> scaner;
    scaner = [&scaner,this](YogaElement& element,ElementOperator& visitor)->void{
        visitor(element);
        for(auto& item : element.children_){
            scaner(*item,visitor);
        }
    };
    scaner(*this,visitor);
    return 0;
}

int32_t YogaLuaLayout::refresh_position_all()
{
    auto width_def = LUA_YG_VALUE(lua_var_,"width");
    auto height_def = LUA_YG_VALUE(lua_var_,"height");

    int width = 1000;
    int height = 1000;


    if(width_def.unit == YGUnitPercent){

    }

    YGNodeCalculateLayout(node_, YGUndefined, YGUndefined, YGDirectionLTR);

    std::function<void(YogaElement& element,float parent_x,float parent_y)> refresher;

    refresher = [&refresher](YogaElement& element,float parent_x,float parent_y)->void{
        element.get_back_position(parent_x,parent_y);
        for(auto& item : element.children_){
            refresher(*item,element.left_,element.top_);
        }
    };
    refresher(*this,0,0);
    return 0;
}

int32_t YogaLuaLayout::refresh_named_elements()
{
    named_elements_.clear();;
    foreach_elements([this](YogaElement& element){
        if(element.id_.size()){
            named_elements_[element.id_] = &element;
        }
    });
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
    MR_TIMER_NEW(t);
    if(dirty_){
        dirty_ = false;
        refresh_position_all();
    }

    if(!renderer_)
        return -1;


    std::function<void(YogaElement& element)> renderer_caller;
    renderer_caller = [&renderer_caller,this](YogaElement& element)->void{
        renderer_->on_render_elements(element);
        for(auto& item : element.children_){
            renderer_caller(*item);
        }
        renderer_->after_render_elements(element);
    };
    renderer_caller(*this);

    //MR_INFO("render frame use {} ms",MR_TIMER_MS(t));
    return 0;
}
