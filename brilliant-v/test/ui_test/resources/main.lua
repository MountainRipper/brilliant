
inspect = require('inspect')
oo = require('bhou.oo.base')

flexDefault = {
    ----flex----
    --flexDirection: row（默认：x轴由左到右）、column（y轴由上到下）、row-reverse（x轴由右到左）、column-reverse（y轴由下到上）
    flexDirection = "column",
    --flexWrap: nowrap（默认：不换行）、wrap（换行）、wrap-reverse（翻转换行，第一行位于下方/右方）
    flexWrap = "no-wrap",
    flexGrow = 0,
    flexShrink = 1,
    flexBasis = nil,
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
    --size etc..,int as pixels,float(0-1.0) as percent
    size = {
      height = nil,
      width = nil
    },
    minimumSize = {
      height = nil,
      width = nil
    },
    maximumSize = {
      height = nil,
      width = nil
    },
    margin = {
      all = nil,
      bottom = nil,
      left = nil,
      right = nil,
      top = nil
    },
    padding = {
      all = nil,
      bottom = nil,
      left = nil,
      right = nil,
      top = nil
    },
    border = {
      all = nil,
      bottom = nil,
      left = nil,
      right = nil,
      top = nil
    },
    position = {
      bottom = nil,
      left = nil,
      right = nil,
      top = nil
    },
}


ui={

main={
    id="root",
    widget="window",

    size={
        width=640,
        height=480
    },

    elements={
        {
            size={
                width=50,
                height=50
            }
        },
        {
            size={
                width=50,
                height=250
            },
            flexGrow=1,
            flexGrow="column",
            elements={
                {
                    id="btn1",
                    widget="button"
                },
                {
                    id="btn2",
                    widget="button"
                },
                {
                    id="btn3",
                    widget="button",
                    onclicked=function ()
                    end
                },
            }
        },
        {
            size={
                width=50,
                height=50
            }
        },
    }
}

}

io.stderr:write(package.path.."\n")
print_dump(flexDefault,"flexDefault")
