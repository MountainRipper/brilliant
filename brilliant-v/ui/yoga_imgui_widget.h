#ifndef YOGAIMGUIWIDGET_H
#define YOGAIMGUIWIDGET_H

#include "imgui_widget_mr.h"
#include "macro_for_each.h"

#define TO_STRING(STR) #STR

#define COMMON_PROPERTY(TYPE,NAME) \
TYPE NAME##_; \
void set_##NAME( const TYPE& value ) { \
        NAME##_ = value; \
} \
TYPE get_##NAME() const { \
        return NAME##_; \
}

#define COMMON_PROPERTY_COPY(TYPE,NAME) \
TYPE NAME##_; \
void set_##NAME( TYPE value ) { \
        NAME##_ = value; \
} \
TYPE get_##NAME() const { \
        return NAME##_; \
}


#define COMPAT_PROPERTY_CASE(NAME)\
        if(key == #NAME) return NAME##_;

#define SOL_PPROPERTY(CLASS,NAME) #NAME,sol::property(&CLASS::get_##NAME, &CLASS::set_##NAME)
#define SOL_PPROPERTY_THIS_CLASS(NAME) #NAME,sol::property(&ThisClass::get_##NAME, &ThisClass::set_##NAME)

#define COMMON_PROPERTIES(...) FOR_EACH_PAIR(COMMON_PROPERTY,__VA_ARGS__)
#define COMMON_PROPERTIES_COPY(...) FOR_EACH_PAIR(COMMON_PROPERTY_COPY,__VA_ARGS__)

#define COMPAT_PROPERTIES_EXPORT(CLASS,...)                 \
using ThisClass = CLASS;                                    \
CompatValue get_value(const std::string_view &key){         \
    MACRO_FOR_EACH(COMPAT_PROPERTY_CASE,,__VA_ARGS__)       \
    return 0;                                               \
}                                                           \
static const std::vector<std::string> property_names(){     \
return std::vector<std::string>{                            \
    FOR_EACH_COMMA(TO_STRING,__VA_ARGS__)                   \
    };                                                      \
}                                                           \
static void bind_lua_class(sol::state& state){              \
    state.new_usertype<CLASS>( #CLASS,                      \
    FOR_EACH_COMMA(SOL_PPROPERTY_THIS_CLASS,__VA_ARGS__)    \
);                                                          \
}

//FOR_EACH_PIVOT_1ST_ARG(SOL_PPROPERTY,CLASS,id,name)

template<typename DataType>
class TypedListModel: public mrui::ListView::AbstractModel{
    // AbstractModel interface
public:
    TypedListModel(){
    }
    virtual int count_imp() override{
        return data_vector_.size();
    }
    virtual std::vector<std::string> keys_imp() override{
        return DataType::property_names();
    }
    virtual CompatValue value_imp(size_t index, const std::string_view &key) override{
        if(index >= data_vector_.size())
            return 0;
        DataType& data = data_vector_[index];
        return data.get_value(key);
    }
    virtual void erase_imp(size_t index) override{
        if(index >= data_vector_.size())
            return;
        data_vector_.erase(data_vector_.begin()+index);
    }
    void append(DataType& data){
        data_vector_.push_back(data);
    }

    typedef std::map<std::string,CompatValue> CompatProperties;
    std::vector<DataType> data_vector_;
};
#endif // YOGAIMGUIWIDGET_H
