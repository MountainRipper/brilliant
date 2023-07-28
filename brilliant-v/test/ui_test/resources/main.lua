
inspect = require('inspect')
oo = require('bhou.oo.base')

flexDefault = {
    ----flex----
    --flexDirection: row（默认：x轴由左到右）、column（y轴由上到下）、row-reverse（x轴由右到左）、column-reverse（y轴由下到上）
    flexDirection = "row",
    --flexWrap: nowrap（默认：不换行）、wrap（换行）、wrap-reverse（翻转换行，第一行位于下方/右方）
    flexWrap = "no-wrap",
    flexGrow = 0,
    flexShrink = 1,
    flexBasis = "auto",
    --direction:inherit,ltr,rtl,
    direction = "inherit",

    ----alignment----
    --justifyContent:主轴对齐 flex-start（默认值：左对齐）、center（居中对齐）、flex-end（右对齐）、space-between（两端对齐）、space-around（每个项目两侧的间距相等）,space-evenly（间距相等，包括边缘）
    justifyContent = "flex-start",
    --alignItems: 交叉轴对齐 stretch(默认值)、flex-start（交叉轴的起点对齐）、center（居中对齐）、flex-end（交叉轴的终点对齐）、baseline(以项目的第一行文字的基线对齐)、space-between（两端对齐）、space-around（每个项目两侧的间距相等）
    alignItems = "stretch",
    --alignContents: 多根轴线的对齐. 如果项目只有一根轴线,该属性不起作用. 与alignItems的值一样
    alignContents = "stretch",
    --alignSelf: 重写自己的对齐,与alignItems的值一样
    alignSelf = "auto",

    ----layout----
    aspectRatio = 0,
    --positionType: relative,absolute
    positionType="relative",

    --value, percent("50%"), "auto"
    height         = nil,
    width          = nil,
    --value and percent("50%"), no auto
    minimumHeight  = nil,
    minimumWidth   = nil,
    maximumHeight  = nil,
    maximumWidth   = nil,

    --value, percent("50%"), "auto"
    marginAll      = nil,
    marginBottom   = nil,
    marginLeft     = nil,
    marginRight    = nil,
    marginTop      = nil,

    --value and percent("50%"), no auto
    paddingAll     = nil,
    paddingBottom  = nil,
    paddingLeft    = nil,
    paddingRight   = nil,
    paddingTop     = nil,

    --value only
    borderAll      = nil,
    borderBottom   = nil,
    borderLeft     = nil,
    borderRight    = nil,
    borderTop      = nil,

    --value only
    positionBottom = nil,
    positionLeft   = nil,
    positionRight  = nil,
    positionTop    = nil
}
keywordsHint={
    style={
        color=0xAABBCCDD,
        colorBackGround=0xAABBCCDD,
        opacity=0.2,
        rotate=0,
        widget="Window"--Button ListView GridView Slider Image ImageButton Rectangle
    },
    Window={
    },
    ChildWindow={
    },
    Text={
    },
    Button={
    },
    Image={
        round=5
    },
    ImageButton={
    },
    Slider={
    },
    ListView={
    },
    GridView={
    },
    Rectangle={
    }
}

oo.class("Layout", BaseClase)
function Layout:init()
    self.ui={}
    self.namedElements={}
end

function Layout:parseNamedElements()
    self.namedElements={}
    local parser = nil
    parser = function(element)
        if(element.id ~= nil)then
            self.namedElements[element.id] = element
            self[element.id] = element
        end
        if(element.elements ~= nil) then
            for i,v  in ipairs(element.elements) do
                parser(v)
            end
        end
    end
    parser(self.ui)
end

function Layout:setElementProperty(id,property,value)
    if(self.namedElements[id] ~= nil) then
        self.namedElements[id][property] = value
        self.nativeDoOperator(self.nativeContext,"dirty",id)
    end
end

function Layout:getElementProperty(id,property)
    if(self.namedElements[id] ~= nil) then
        return self.namedElements[id][property]
    end
    return nil
end

function Layout:setElementStyle(id,style,value)
    if(self.namedElements[id] ~= nil) then
        if(self.namedElements[id].style ~= nil) then
            self.namedElements[id].style[style] = value
        end
    end
end
function Layout:getElementStyle(id,style)
    if(self.namedElements[id] ~= nil) then
        if(self.namedElements[id].style ~= nil) then
            return self.namedElements[id].style[style]
        end
    end
    return nil
end

oo.class("ImageButtonRow",Layout)
function ImageButtonRow:init()
    self.ui={
        height="auto",
        width="100%",
        flexDirection="row",
        flexWrap="wrap",
        elements={
            {
                width=128,
                height=128,
                widget="ImageButton",
                style={
                    image="dialog-apply.png",colorButton=0xFF8800FF,colorButtonHovered=0xFFAA00FF,colorButtonActive=0xFF8800FF
                }
            },
            {
                width=128,
                height=128,
                marginLeft=20,
                widget="ImageButton",
                style={
                    image="wechat.png",colorButton=0xCCCCCCFF,colorButtonHovered=0xFFFFFFFF,colorButtonActive=0xCCCCCCFF
                }
            },
            {
                width=128,
                height=128,
                marginLeft=20,
                widget="ImageButton",
                style={
                    image="wechat.png",colorButton=0xCCCCCCFF,colorButtonHovered=0xFFFFFFFF,colorButtonActive=0xCCCCCCFF,colorTint=0xFFFF00FF
                }
            },
            {
                width=128,
                height=128,
                marginLeft=20,
                widget="RoundImageButton",
                style={
                    image="wechat.png",colorButton=0xFFFFFFFF
                }
            },
            {
                width=128,
                height=128,
                marginLeft=10,
                widget="RoundImageButton",
                style={
                    image="wechat.png",colorButton=0xFFFFFFFF,frameRounding=64
                },
                elements ={
                    {
                        width="100%",
                        height="100%",
                        widget="Text",
                        positionType="absolute",
                        style={
                            text="Text",
                            aliginH = "center",
                            aliginV = "center",
                            color=0xFF8800FF
                        }
                    }
                },
            },
            {
                width=128,
                height=128,
                marginLeft=10,
                widget="RoundImageButton",
                style={
                    image="girl.png",colorButton=0xFFFFFFFF,frameRounding=64
                }
            },
            {
                width=128,
                height=128,
                marginLeft=10,
                widget="RoundImageButton",
                style={
                    image="girl.png",colorButton=0xFFFFFFFF,frameRounding=64,colorTint=0xFFFF55FF
                }
            },
            {
                width=128,
                height=128,
                marginLeft=10,
                widget="RoundImageButton",
                style={
                    image="effect.png",colorButton=0xFFFFFFFF
                }
            },
            {
                id="btnEffect",
                width=128,
                height=128,
                marginLeft=10,
                widget="RoundImageButton",
                style={
                    image="effect.png",colorButton=0xFFFFFFFF,colorTint=0x9A45F0FF
                }
            },
        }
    }
end

oo.class("MainUI", Layout)
function MainUI:init()

    self.ui={
        id="root",        
        widget="Window",
        style={
        },

        width=1024,
        height="auto",

        elements={
            {
                widget="ChildWindow",
                width="100%",
                height="100%",
                flexDirection="column",
                style={dragScrollX=true,dragScrollY=true},
                elements={
                    {
                        width=10,
                        height=40,
                        widget="Text",
                        style={text="Images"}
                    },
                    {
                        height=128,
                        width="100%",
                        flexDirection="row",
                        elements={
                            {
                                width=128,
                                height=128,
                                widget="Image",
                                style={
                                    image="wechat.png",color=0xFFFFFFFF
                                }
                            },
                            {
                                width=128,
                                height=128,
                                widget="Image",
                                marginLeft=8,
                                style={
                                    image="effect.png",color=0xFFFFFFFF
                                }
                            },
                            {
                                width=128,
                                height=128,
                                widget="Image",
                                marginLeft=8,
                                style={
                                    image="effect.png",color=0xFF8800FF
                                }
                            },
                            {
                                width=128,
                                height=128,
                                widget="Image",
                                marginLeft=8,
                                style={
                                    image="girl.png",color=0xFFFFFFFF
                                },
                                elements ={
                                    {
                                        width="100%",
                                        height="100%",
                                        widget="Text",
                                        positionType="absolute",
                                        style={
                                            text="Origin",
                                            color=0xFF0000FF,
                                            aliginH = "center",
                                            aliginV = "center"
                                        }
                                    }
                                },
                            },
                            {
                                width=128,
                                height=128,
                                widget="Image",
                                marginLeft=8,
                                style={
                                    image="girl.png",color=0xFFFFFFFF,frameRounding=64
                                },
                                elements ={
                                    {
                                        width="100%",
                                        height="100%",
                                        widget="Text",
                                        positionType="absolute",
                                        style={
                                            text="Round",
                                            color=0xFF0000FF,
                                            aliginH = "center",
                                            aliginV = "center"
                                        }
                                    }
                                },
                            },
                            {
                                width=128,
                                height=128,
                                widget="Image",
                                marginLeft=8,
                                style={
                                    image="girl.png",color=0xFFFFFF33,frameRounding=32
                                },
                                elements ={
                                    {
                                        width="100%",
                                        height="100%",
                                        widget="Text",
                                        positionType="absolute",
                                        style={
                                            text="Alpha",
                                            color=0xFF0000FF,
                                            aliginH = "center",
                                            aliginV = "center"
                                        }
                                    }
                                },
                            },
                            {
                                width=128,
                                height=128,
                                widget="Image",
                                marginLeft=8,
                                style={
                                    image="girl.png",color=0xFF99FFFF
                                },
                                elements ={
                                    {
                                        width="100%",
                                        height="100%",
                                        widget="Text",
                                        positionType="absolute",
                                        style={
                                            text="TintColor",
                                            color=0xFF0000FF,
                                            aliginH = "center",
                                            aliginV = "center"
                                        }
                                    }
                                },
                            }
                        }
                    },
                    {
                        width=10,
                        height=40,
                        widget="Text",
                        marginTop=10,
                        style={text="Buttons"}
                    },
                    {
                        height=50,
                        width="100%",
                        flexDirection="row",
                        elements={
                            {
                                id="btn1",
                                width=100,
                                height=50,
                                widget="Button",
                                style={
                                    text="Play",colorButton=0xFF000099,colorButtonHovered=0xFF0000AA,colorButtonActive=0xFF000055
                                },
                                onclicked = function(this)
                                    self:setElementProperty("btnEffect","width",self:getElementProperty("btnEffect","width")+20)
                                    self:setElementProperty("btnEffect","height",self:getElementProperty("btnEffect","height")+20)
                                end
                            },
                            {
                                id="btn2",
                                width=300,
                                height=50,
                                marginLeft=10,
                                marginRight=10,
                                widget="Button",
                                style={
                                    text="Pause",colorButton=0x00FF0099,colorButtonHovered={0,1,0,0.8},colorButtonActive=0x00FF0055
                                },
                                onclicked = function(this)
                                    if(this.width > 100) then
                                        self:setElementProperty("btn2","width",self:getElementProperty("btn2","width")-20)
                                    end
                                end
                            },
                            {
                                id="btn3",
                                width=100,
                                height=50,
                                widget="Button",
                                style={
                                    text="Stop",colorButton=0x0055FFFF,colorButtonHovered=0x0055FFAA,colorButtonActive={0,128,255,200},frameRounding=16
                                },
                                onclicked = function(this)
                                    self:setElementProperty("btn3","width",self:getElementProperty("btn3","width")+20)
                                end
                            },
                            {
                                id="btn4",
                                width=200,
                                height=50,
                                marginLeft=10,
                                marginRight=10,
                                widget="Button",
                                style={
                                    text="Info",colorButton={1.0,1.0,1.0,1.0,255},colorButtonHovered={0.4,0.4,0.4,1},colorButtonActive={0.3,0.3,0.3,1},frameRounding=8
                                }
                            },
                        }
                    },
                    {
                        width=10,
                        height=40,
                        widget="Text",
                        marginTop=10,
                        style={text="ImageButtons",color=0xFF8800FF},
                    },                    
                    {
                        id="imageButtonRow",
                        class=classOf(ImageButtonRow)
                    },
                    {
                        width=10,
                        height=40,
                        widget="Text",
                        marginTop=10,
                        style={text="Rectangles",color=0xFF8800FF},
                    },
                    {
                        height=128,
                        width="100%",
                        flexDirection="row",
                        elements={
                            {
                                widget="RectangleFrame",
                                width = "20%",
                                height = "80%",
                                style={
                                    color=0xFF8800FF,frameWidth=4
                                },
                            },
                            {
                                widget="RectangleFrame",
                                width = "20%",
                                height = "80%",
                                marginLeft=16,
                                style={
                                    color=0xFF8800FF,frameRounding=20,frameWidth=8
                                },
                            },
                            {
                                widget="Rectangle",
                                width = "20%",
                                height = "80%",
                                marginLeft=16,
                                style={
                                    color=0xFF8800FF,frameWidth=4
                                },
                            },
                            {
                                widget="Rectangle",
                                width = "20%",
                                height = "80%",
                                marginLeft=16,
                                style={
                                    color=0xFF880088,frameRounding=20,frameWidth=4
                                },
                            },
                        }
                    },
                    {
                        width=10,
                        height=40,
                        widget="Text",
                        style={text="Circel",color=0xFF8800FF},
                    },
                    {
                        height=128,
                        width="100%",
                        flexDirection="row",
                        elements={
                            {
                                widget="CircelFrame",
                                width = 128,
                                height = 128,
                                style={
                                    color=0xFF8800FF,frameRounding=64,frameWidth=4
                                },
                            },
                            {
                                widget="Circel",
                                width = 128,
                                height = 128,
                                marginLeft=16,
                                style={
                                    color=0xFF8800FF,frameRounding=64
                                },
                            }
                        }
                    },

                    {
                        width=10,
                        height=40,
                        widget="Text",
                        marginTop=10,
                        style={text="TextAligin",color=0xFF8800FF},
                    },
                    {
                        width="50%",
                        height=120,
                        widget="Rectangle",
                        style={
                            color=0xFF8800FF
                        },
                        elements ={
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="TC",
                                    aliginH = "left",
                                    aliginV = "top"
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="LC",
                                    aliginH = "left",
                                    aliginV = "center",
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="LB",
                                    aliginH = "left",
                                    aliginV = "bottom"
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="CT",
                                    aliginH = "center",
                                    aliginV = "top"
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="Center",
                                    aliginH = "center",
                                    aliginV = "center"
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                style={
                                    text="CB",
                                    aliginH = "center",
                                    aliginV = "bottom"
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="RT",
                                    aliginH = "right",
                                    aliginV = "top"
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="RC",
                                    aliginH = "right",
                                    aliginV = "center"
                                }
                            },
                            {
                                width="100%",
                                height="100%",
                                widget="Text",
                                positionType="absolute",
                                style={
                                    text="RB",
                                    aliginH = "right",
                                    aliginV = "bottom"
                                }
                            }
                        }
                    },
                    {
                        width=10,
                        height=40,
                        widget="Text",
                        marginTop=10,
                        style={text="Input Text",color=0xFF8800FF},
                    },
                    {
                        height=50,
                        width=256,
                        marginTop=10,
                        widget="InputText",
                        --style={lable="InputText"},
                        valueChanged=function(this)
                            io.stderr:write("TextInput value changed:"..this.nativeContext.string.."\n")
                        end
                    },
                    {
                        height=256,
                        width=256,
                        marginTop=10,
                        widget="InputTextMultiline",
                        --style={lable="InputTextMultiline"},
                        valueChanged=function(this)
                            io.stderr:write("InputTextMultiline value changed:"..this.nativeContext.string.."\n")
                        end
                    },
                    {
                        width=10,
                        height=40,
                        widget="Text",
                        marginTop=10,
                        style={text="Slider",color=0xFF8800FF},
                    },
                    {
                        height=50,
                        width=256,
                        marginTop=10,
                        widget="Slider",
                        style={valueMin=1.0,valueMax=10.0},
                        valueChanged=function(this)
                            io.stderr:write("Slider value changed:"..this.nativeContext.number.."\n")
                        end
                    },
                }
            }
        }
    }
end


oo.class("UserDelegateTest",Layout)
function UserDelegateTest:init()
self.ui={
    id="rootDelegate",
    flexDirection="row",
    alignItems="center",
    width="100%",
    height=50,
    elements={
        {
            widget="Text",
            width=50,
            height=50,
            style={text=model.id}
        },
        {
            widget="Image",
            width=64,
            height=64,
            style={image="blueman-trust.png"}
        },
        {
            width="auto",
            height=1,
            flexGrow=1,
        },
        {
            widget="Button",
            width="auto",
            height=50,
            style={text=self.model.name}
        }
    }
}
end

function delegateOf(Object)
    return function()
        oo.new(Object)
    end
end

oo.class("RecordUI", Layout)
function RecordUI:init()
self.ui={
    id="recorder",
    flexDirection="row",
    widget="Window",
--    width="100%",
    width=720,
    height=1280,
    elements={
        {
            widget="ListView",
            widget=400,
            height=400,
            style={
                --the model must set before ListView construct
                model=testModel,
                delegate=delegateOf(UserDelegateTest)
            }
        }
    }
}
end

registerWidgets("org.mr.brilliant.MainUI", MainUI)
registerWidgets("org.mr.brilliant.RecordUI", RecordUI)

--print_dump(MainUI,"MainUI")
--print_dump(flexDefault,"flexDefault")
--print_dump(keywordsHint,"keywordsHint")

