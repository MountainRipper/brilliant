#ifndef YOGALUALAYOUT_H
#define YOGALUALAYOUT_H
#include <string>
#include <map>
#include <variant>
#include <vector>
#include <functional>
#include <sol/sol.hpp>
class YGNode;
class YogaElement;

typedef std::variant<double,bool,std::string,std::vector<double>,std::vector<std::string>,void*> LuaCompatValue;
typedef std::function<void(YogaElement&)> ElementOperator;

#define kCompatValueNumberIndex 0
#define kCompatValueBooleanIndex 1
#define kCompatValueStringIndex 2
#define kCompatValueNumberArrayIndex 3
#define kCompatValueStringArrayIndex 4
#define kCompatValuePointerIndex 5

#define STYLE_VALUE_GETTER(MAP,TYPE,TYPE_DEFAULT,VAR_INDEX) \
TYPE style_value(const std::string& name,TYPE_DEFAULT default_value,bool* exist = nullptr){ \
    if(exist) *exist = false; \
    auto it = MAP.find(name); \
    if(it == MAP.end()) \
        return default_value; \
    if(it->second.index() != VAR_INDEX) \
        return default_value; \
    if(exist) *exist = true; \
    return std::get<TYPE>(it->second); \
}

#define STYLE_VALUE_GETTER_REF(MAP,TYPE,TYPE_DEFAULT,VAR_INDEX) \
TYPE style_value(const std::string& name,const TYPE_DEFAULT& default_value,bool* exist = nullptr){ \
    if(exist) *exist = false; \
    auto it = MAP.find(name); \
    if(it == MAP.end()) \
        return default_value; \
    if(it->second.index() != VAR_INDEX) \
        return default_value; \
    if(exist) *exist = true; \
    return std::get<TYPE>(it->second); \
}

#define STYLE_VALUE_GETTER_ALIAS(MAP,TYPE,TYPE_ALIAS,VAR_INDEX) \
TYPE_ALIAS style_value(const std::string& name,TYPE_ALIAS default_value,bool* exist = nullptr){ \
    if(exist) *exist = false; \
    auto it = MAP.find(name); \
    if(it == MAP.end()) \
        return default_value; \
    if(it->second.index() != VAR_INDEX) \
        return default_value; \
    if(exist) *exist = true; \
    return static_cast<TYPE_ALIAS>(std::get<TYPE>(it->second)); \
}

#define LUA_PROPERTY_GET(TYPE,NAME,DEFAULT) \
TYPE get_##NAME() const { \
    TYPE v = DEFAULT; \
    auto& type_info = typeid(TYPE); \
    auto index = value_.index(); \
    if((index == kCompatValueNumberIndex         && type_info == typeid(double)) || \
       (index == kCompatValueBooleanIndex        && type_info == typeid(bool))   || \
       (index == kCompatValueStringIndex         && type_info == typeid(std::string)) || \
       (index == kCompatValueNumberArrayIndex    && type_info == typeid(std::vector<double>)) || \
       (index == kCompatValueStringArrayIndex    && type_info == typeid(std::vector<std::string>)) || \
       (index == kCompatValuePointerIndex        && type_info == typeid(void*)) ) \
            v = std::get<TYPE>(value_); \
    return v; \
}
#define LUA_PROPERTY_SET(TYPE,NAME) \
void set_##NAME( const TYPE& value ) { \
    value_ = value; \
}
#define LUA_PROPERTY_SET_NO_REF(TYPE,NAME) \
void set_##NAME( TYPE value ) { \
    value_ = value; \
}

#define LUA_PROPERTY(TYPE,NAME,DEFAULT) \
LUA_PROPERTY_GET(TYPE,NAME,DEFAULT) \
LUA_PROPERTY_SET(TYPE,NAME)

#define LUA_PROPERTY_NO_REF(TYPE,NAME,DEFAULT) \
LUA_PROPERTY_GET(TYPE,NAME,DEFAULT) \
LUA_PROPERTY_SET_NO_REF(TYPE,NAME)

#define UINT32_SWAP_ENDIAN(u32) u32 = (u32 >> 24) | ((u32<<8) & 0x00FF0000) | ((u32>>8) & 0x0000FF00) | (u32 << 24);

class noncopyable
{
private:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

//YogaElement's lua var hold a native context, in constructor
//so multi instance will not allowed,the lua var do not known which native object to use
class YogaElement : noncopyable{
public:
    YogaElement(sol::table &lua_var);
    int32_t push_element(std::shared_ptr<YogaElement> element);
    int32_t parse_self_layout();
    int32_t parse_self_widget();
    int32_t get_back_position(float parent_x = 0, float parent_y = 0);
    int32_t get_back_position_recursion();

    STYLE_VALUE_GETTER(styles_,double,double,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,int8_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,int16_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,int32_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,int64_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,uint8_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,uint16_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,uint32_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER_ALIAS(styles_,double,uint64_t,kCompatValueNumberIndex)
    STYLE_VALUE_GETTER(styles_,bool,bool,kCompatValueBooleanIndex)
    STYLE_VALUE_GETTER_REF(styles_,std::string,std::string,kCompatValueStringIndex)
    STYLE_VALUE_GETTER(styles_,std::string,const char*,kCompatValueStringIndex)
    STYLE_VALUE_GETTER_REF(styles_,std::vector<double>,std::vector<double>,kCompatValueNumberArrayIndex)
    STYLE_VALUE_GETTER_REF(styles_,std::vector<std::string>,std::vector<std::string>,kCompatValueStringArrayIndex)
    STYLE_VALUE_GETTER(styles_,void*,void*,kCompatValuePointerIndex)

    LUA_PROPERTY(double,number,0)
    LUA_PROPERTY(bool,boolean,false)
    LUA_PROPERTY(std::string,string,"")
    LUA_PROPERTY(std::vector<double>,number_array,{})
    LUA_PROPERTY(std::vector<std::string>,string_array,{})
    LUA_PROPERTY_NO_REF(void*,pointer,nullptr)

    template <typename PointType>
    PointType left_top(){
        return PointType(left_,top_);
    }
    template <typename PointType>
    PointType right_bottom(){
        return PointType(left_+width_, top_+height_);
    }
    template <typename PointType>
    PointType size(){
        return PointType(width_,height_);
    }

    void emit_event(const std::string& event);

    uint32_t style_color(const std::string& style,uint32_t defalt_value = 0xFF000000,bool* exist = nullptr);
    uint32_t style_value_color(const LuaCompatValue& value, uint32_t defalt_value = 0xFF000000);
public:
    static int32_t parse_node(sol::table& element_table, YGNode *node);
    static int32_t parse_widget(sol::table& element_table, YogaElement &element_self);

public:
    YGNode*     node_   = 0;
    sol::table  lua_var_;

    std::string widget_;
    std::string id_;
    float       left_     = 0;
    float       top_      = 0;
    float       width_    = 0;
    float       height_   = 0;

    std::vector<std::shared_ptr<YogaElement>>       children_;
    std::unordered_map<std::string,LuaCompatValue>  styles_;
    LuaCompatValue value_;
};

class YogaLuaLayoutRenderer{
public:
    virtual int32_t on_render_elements(YogaElement& element) = 0;
    virtual int32_t after_render_elements(YogaElement &element) = 0;
};


class YogaLuaLayout : public YogaElement
{
public:
    YogaLuaLayout(sol::table &lua_object, sol::table &lua_ui);

    YogaLuaLayout* clone(sol::table& self);
    int32_t set_renderer(YogaLuaLayoutRenderer* renderer);
    int32_t refresh_position_all();
    int32_t refresh_named_elements();

    int32_t render_frame();

    int32_t foreach_elements(ElementOperator visitor);

    int32_t lua_set_element_property(const std::string &id, const std::string &property, sol::lua_value value);
    int32_t lua_do_operator(const std::string &method, sol::lua_value value);
private:
    int32_t parse_child_element_recursion();
private:
    YogaLuaLayoutRenderer* renderer_ = nullptr;
    sol::table object_;
    std::map<std::string,YogaElement*> named_elements_;
    bool dirty_ = false;
};

#endif // YOGALUALAYOUT_H
