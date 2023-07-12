#ifndef YOGALUALAYOUT_H
#define YOGALUALAYOUT_H
#include <string>
#include <map>
#include "yoga/YGNode.h"

class YGNode;

class YogaElement{
    std::map<std::string,YogaElement> children;
private:
    YGNode* node_;
};

class YogaLuaLayout : public YogaElement
{
public:
    YogaLuaLayout();
private:
};

#endif // YOGALUALAYOUT_H
