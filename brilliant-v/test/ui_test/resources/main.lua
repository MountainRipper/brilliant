
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
    self.layout={}
end

function Layout:setElementProperty(id,property,value)

end

oo.class("MainUI", Layout)
function MainUI:init()
    self.layout={
        id="root",
        flexDirection="column",
        widget="Window",
        style={
        },

        width="100%",
        height="100%",

        elements={
            {
                id="L",
                width=128,
                height=128,
                widget="Image",
                style={
                    image="document-page-setup.png",opacity=0.5,color=0xFF0000FF,colorBackGround=0x88FF00FF
                }
            },
            {
                id="C",
                height=50,
                alignItems="center",
                marginTop=10,
                marginBottom=10,
--                flexDirection="column",
                elements={
                    {
                        id="btn1",
                        width=100,
                        height=50,
                        widget="Button",
                        style={
                            text="Play",opacity=0.5,color=0xFF0000FF,colorBackGround=0x88FF00FF
                        }
                    },
                    {
                        id="btn2",
                        width=200,
                        height=50,
                        marginLeft=10,
                        marginRight=10,
                        widget="Button",
                        style={
                            text="Pause",opacity=0.5,color=0xFF0000FF,colorBackGround=0x88FF00FF
                        }
                    },
                    {
                        id="btn3",
                        width=300,
                        height=50,
                        widget="Button",
                        style={
                            text="Stop",opacity=0.5,color=0xFF0000FF,colorBackGround=0x88FF00FF
                        },
                        onclicked=function ()
                            io.stderr:write(">>>>>>>>>>>>>>>>>>>>>>:"..self.layout.id.."\n")
                        end
                    },
                }
            },
            {
                height=50,
                width=256,
                widget="Slider",
            },
            {
                id="R",
                width=128,
                height=128,
                widget="ImageButton",
                style={
                    image="mail-message-new.png",opacity=0.5,color=0xFF0000FF,colorBackGround=0x88FF00FF
                }
            },
        }
    }
end


registerWidgets("org.mr.brilliant.MainUI", MainUI)

print_dump(MainUI,"MainUI")
print_dump(flexDefault,"flexDefault")
print_dump(keywordsHint,"keywordsHint")

test = {
    auto="auto",
    percent="45%",
    num=3.1415926
}
