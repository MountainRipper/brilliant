
#include "mr_im_widget.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include "imgui_yoga_render.h"
#include <mrcommon/logger.h>
ImGuiYogaRender::ImGuiYogaRender()
{

}


int32_t ImGuiYogaRender::on_render_elements(YogaElement &element)
{
    MR_INFO("render wiget:{} id:{} pos:{}#{}#{}#{} size:{}x{}",element.widget_,element.id_,
             element.top_,element.left_,element.bottom_,element.right_,
             element.width_,element.height_);

    if(element.widget_.empty()){
        return 0;
    }

    ImVec2 size(element.width_,element.height_);

    if(element.widget_ == "Window"){
        ImGui::Begin(element.id_.c_str());
    }

    ImGui::SetCursorPos(ImVec2(element.left_,element.top_) + ImGui::GetWindowContentRegionMin() );
    if(element.widget_ == "Button"){
        ImGui::Button(std::get<std::string>(element.properties_["text"]).c_str(),size);
    }
    else if(element.widget_ == "Image"){
        mrui::Image(std::get<std::string>(element.properties_["image"]).c_str(),NULL,size);ImGui::SameLine();
    }
    else if(element.widget_ == "ImageButton"){
        mrui::ImageButton(element.id_.c_str(),std::get<std::string>(element.properties_["image"]).c_str(),NULL,size);ImGui::SameLine();
    }
    else if(element.widget_ == "Slider"){
        //mrui::Slid(element.id_.c_str(),std::get<std::string>(element.properties_["image"]).c_str(),NULL,size);ImGui::SameLine();
        static float a = 0;
        ImGui::SetNextItemWidth(element.width_);
        ImGui::SliderFloat(element.id_.c_str(),&a,0,100);
    }
    return 0;
}

int32_t ImGuiYogaRender::after_render_elements(YogaElement &element)
{
    if(element.widget_ == "Window"){
        ImGui::End();
    }
    return 0;
}
