#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H
#include <string>
#include <typeinfo>
#include <variant>
#include <vector>

typedef std::variant<double,bool,std::string,std::vector<double>,std::vector<std::string>,void*> CompatValueType;
#define kCompatValueNumberIndex 0
#define kCompatValueBooleanIndex 1
#define kCompatValueStringIndex 2
#define kCompatValueNumberArrayIndex 3
#define kCompatValueStringArrayIndex 4
#define kCompatValuePointerIndex 5

#define CompatValueTo(VALUE,TYPE,TYPE_AS,TYPE_INDEX,FEFAULT)\
operator TYPE_AS const() const{\
    if(VALUE.index() == TYPE_INDEX)\
        return std::get<TYPE>(VALUE);\
    return FEFAULT;\
}\
CompatValue& operator = (TYPE_AS v){ \
    VALUE = static_cast<TYPE>(v); \
    return *this; \
} \
CompatValue(TYPE_AS v){ \
    VALUE = static_cast<TYPE>(v); \
}

#define CompatValueRef(VALUE,TYPE,NAME) \
TYPE& ref_##NAME(){ \
    return std::get<TYPE>(VALUE); \
}

class CompatValue{
public:
    CompatValue(){}
    CompatValue(CompatValueType value)
        :value_(value){
    }

    int index() const{
        return value_.index();
    }
    CompatValueTo(value_,double,double,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,float,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,int8_t ,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,int16_t,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,int32_t,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,int64_t,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,uint8_t ,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,uint16_t,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,uint32_t,kCompatValueNumberIndex,0)
    CompatValueTo(value_,double,uint64_t,kCompatValueNumberIndex,0)

    CompatValueTo(value_,bool,bool,kCompatValueBooleanIndex,0)
    CompatValueTo(value_,std::string,std::string,kCompatValueStringIndex,"")
    CompatValueTo(value_,std::vector<double>,std::vector<double>,kCompatValueNumberArrayIndex,{})
    CompatValueTo(value_,std::vector<std::string>,std::vector<std::string>,kCompatValueNumberArrayIndex,{})
    CompatValueTo(value_,void*,void*,kCompatValuePointerIndex,nullptr)

    CompatValueRef(value_,double,number)
    CompatValueRef(value_,bool,boolean)
    CompatValueRef(value_,std::string,string)
    CompatValueRef(value_,std::vector<double>,number_array)
    CompatValueRef(value_,std::vector<std::string>,string_array)
    CompatValueRef(value_,void*,pointer)

    CompatValueType value_;
};

#define GETTER_SETTER(TYPE,NAME) \
TYPE NAME##_; \
void set_##NAME( const TYPE& value ) { \
    NAME##_ = value; \
} \
TYPE get_##NAME() const { \
    return NAME##_; \
}

#define GETTER_SETTER_NO_REF(TYPE,NAME) \
TYPE NAME##_; \
void set_##NAME( TYPE value ) { \
        NAME##_ = value; \
} \
TYPE get_##NAME() const { \
    return NAME##_; \
}

#define GETTER_RETURN(DATA,NAME,KEY) \
if(KEY == #NAME)\
    return DATA.get_##NAME();


#endif // BASIC_TYPES_H
