#include "mr_im_widget.h"
#include <imgui/imgui_internal.h>

#if defined(MR_UI_WITH_TIO)
#include <tio/tio_hardware_graphic.h>
#endif

using namespace ImGui;
namespace mrui {

std::map<std::string,TextureHolder::Image> TextureHolder::images_;

void VideoCutTimeline::show(float height)
{
    auto parent_window = GetCurrentWindow();
    PushID(this);
    ImVec2 size = ImVec2(totle_width_*2,height);

    SetCursorPosX(offset_x_);
    BeginChild("child",size,false,ImGuiWindowFlags_NoScrollbar);

    int index = 0;

    ImVec4 colors[] = {ImVec4{1,0.7,0.2,1},ImVec4{0.8,0.6,1,1},ImVec4{0.4,0.75,1,1},ImVec4{1,0.7,0.7,1}};

    std::vector<int> fragment_width_this_frame;
    bool refresh_width = false;
    index = 0;
    for(auto& item : fragments_){
        PushID(index);
        int range_width = item.range_width(pixel_per_second_);
        fragment_width_this_frame.push_back(range_width);
        ColorButton(item.name_.c_str(),colors[index],0,ImVec2(range_width,height));
        //InvisibleButton(item.name_.c_str(),ImVec2(item.width_,height));
        bool active = IsItemActive();
        if(active){
            if(IsMouseDown(0) && !mouse_down_){
                //need mark mouse_down_, only check first mouse down pos, so move mouse fast will not horver in head/tail area
                mouse_down_ = true;
                if(GetIO().MousePos.x >= GetItemRectMin().x && GetIO().MousePos.x <= GetItemRectMin().x + range_holder_pixels_){
                    if(!item.drag_start_)
                        fprintf(stderr,"Drag Start %s\n",item.name_.c_str());
                    item.drag_start_ = true;
                    refresh_width = true;
                }
                if(GetIO().MousePos.x >= GetItemRectMax().x - range_holder_pixels_ && GetIO().MousePos.x <= GetItemRectMax().x){
                    if(!item.drag_end_)
                        fprintf(stderr,"Drag End %s\n",item.name_.c_str());
                    item.drag_end_ = true;
                    refresh_width = true;
                }
                select_item = index;
            }
            if (IsMouseDragging(0)){
                int delta_x = GetIO().MouseDelta.x;
                if(item.drag_start_){
                    range_width -= delta_x;

                    if(range_width < pixel_per_second_*4)
                        range_width = pixel_per_second_*4;
                    else if(range_width > item.width_)
                        range_width = item.width_;
                    else
                        offset_x_ += delta_x;

                    float duration_ms = range_width * 1000.0 /  pixel_per_second_;
                    item.range_ = ImVec2(item.range_.y - duration_ms,item.range_.y);
                }
                else if(item.drag_end_){
                    range_width += delta_x;

                    if(range_width < pixel_per_second_*4)
                        range_width = pixel_per_second_*4;
                    else if(range_width > item.width_)
                        range_width = item.width_;

                    float duration_ms = range_width * 1000.0 /  pixel_per_second_;
                    item.range_ = ImVec2(item.range_.x, item.range_.x + duration_ms);
                }
                else{
                    offset_x_ += GetIO().MouseDelta.x;
                }
                fprintf(stderr,"%s select range:%f-%f\n",item.name_.c_str(),item.range_.x,item.range_.y);
            }
        }
        if(IsMouseReleased(0)){
            item.drag_start_ = false;
            item.drag_end_ = false;
            mouse_down_ = false;
            refresh_width = true;
        }

        SameLine(0,0);
        PopID();
        index++;
    }
    if(refresh_width){
        totle_width_ = 0;
        for(auto& item : fragments_){
            totle_width_ += item.width_;
        }
    }


    auto content_width = parent_window->Size.x;
    if(offset_x_ < -(totle_width_ - 200))
        offset_x_ = -(totle_width_ - 200);
    else if(offset_x_ > (content_width - 200) )
        offset_x_ = (content_width - 200);


    auto window = GetCurrentWindow();
    auto image_l = TextureHolder::get("timeline_handle_left.png");
    auto image_r = TextureHolder::get("timeline_handle_right.png");
    int32_t x = 0;
    index = 0;
    ImVec2 pos = window->Pos;
    for(auto& item : fragments_){
        if(select_item == index)
            window->DrawList->AddImage(image_l.texture,
                                       pos + ImVec2(x,0),
                                       pos + ImVec2(x+range_holder_pixels_,0+height),
                                       image_l.top_left(),
                                       image_l.bottom_right());

        x += fragment_width_this_frame[index];

        if(select_item == index)
            window->DrawList->AddImage(image_r.texture,
                                       pos + ImVec2(x-range_holder_pixels_,0),
                                       pos + ImVec2(x,0+height),
                                       image_r.top_left(),
                                       image_r.bottom_right());

        index++;
    }

    EndChild();
    PopID();



}

TextureHolder::Image TextureHolder::get(const std::string &name)
{
    Image image;
    if(images_.find(name) != images_.end())
        image = images_[name];
    return image;
}

int32_t TextureHolder::add_image(const std::string & name, Image image)
{
    images_[name] = image;
    return 0;
}

const std::map<std::string, TextureHolder::Image> &TextureHolder::all_images()
{
    return images_;
}
#if defined(MR_UI_WITH_TIO)
std::map<std::string,mr::tio::GraphicTexture>  TextureHolder::textures_;

int32_t TextureHolder::add_image(const std::string &image_file)
{
    mr::tio::GraphicTexture texture;
    std::map<std::string,mr::tio::FrameArea> areas;
    mr::tio::TextureIO::create_texture(image_file,[](const std::string& image_file,
                                                      mr::tio::GraphicTexture& texture,
                                                      std::map<std::string,mr::tio::FrameArea>& areas){
        if(textures_.find(image_file) != textures_.end()){
            mr::tio::TextureIO::release_texture(textures_[image_file]);
            textures_.erase(image_file);
        }
        textures_[image_file] = texture;
        for(auto& image : areas){
            TextureHolder::Image image_info;
            image_info.texture = (ImTextureID)texture.context[0];
            image_info.pos = ImVec2(image.second.x,image.second.y);
            image_info.size = ImVec2(image.second.width,image.second.height);
            image_info.size_totle = ImVec2(texture.width,texture.height);
            add_image(image.first,image_info);
        }
    });
    return areas.size();
}

void Image(const char* image,const char* sub_image,const ImVec2& size, const ImVec4& tint_col , const ImVec4& border_col ){
    std::string image_key = image;
    if(sub_image)
        image_key = image_key + ":" + sub_image;
    auto image_info = TextureHolder::get(image_key);
    ImGui::Image(image_info.texture,size,image_info.top_left(),image_info.bottom_right(),tint_col,border_col);
}
bool ImageButton(const char* str_id, const char* image,const char* sub_image, const ImVec2& size, const ImVec4& bg_col, const ImVec4& tint_col){    
    std::string image_key = image;
    if(sub_image)
        image_key = image_key + ":" + sub_image;
    auto image_info = TextureHolder::get(image_key);

    return ImGui::ImageButton(str_id,image_info.texture,size,image_info.top_left(),image_info.bottom_right(),bg_col,tint_col);
}
#endif

}//namespace mountain-ripper
