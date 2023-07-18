#include "mr_im_widget.h"
#include <imgui/imgui_internal.h>
#include <queue>

#if defined(MR_UI_WITH_TIO)
#include <tio/tio_hardware_graphic.h>
#endif

using namespace ImGui;
namespace mrui {

#define ARRARY_SUM(arr,count,sum) {size_t c = count;while((c--) > 0) sum += arr[c];}
#define ARRARY_SUM_CLEAR(arr,count,sum) {size_t c = count;while((c--) > 0) sum += arr[c]; arr[c]=0;}

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
    auto pos = name.find_first_of(':') + 1;
    if( pos != std::string::npos && pos != name.size()){
        std::string sub_name(name.begin()+pos,name.end());
        images_[sub_name] = image;
    }
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

void ImageRound(const char *image, const char *sub_image, const ImVec2 &size, float round, const ImVec4 &tint_col, const ImVec4 &border_col)
{
    std::string image_key = image;
    if(sub_image)
        image_key = image_key + ":" + sub_image;
    auto image_info = TextureHolder::get(image_key);
    auto min = ImGui::GetCursorPos() + ImGui::GetCurrentWindow()->Pos;
    auto max = min + size;
    ImGui::GetWindowDrawList()->AddImageRounded(image_info.texture,min,max,image_info.top_left(),image_info.bottom_right(),ImGui::ColorConvertFloat4ToU32(tint_col),round);
}

bool ImageButton(const char* str_id, const char* image,const char* sub_image, const ImVec2& size, const ImVec4& bg_col, const ImVec4& tint_col){    
    std::string image_key = image;
    if(sub_image)
        image_key = image_key + ":" + sub_image;
    auto image_info = TextureHolder::get(image_key);

    return ImGui::ImageButton(str_id,image_info.texture,size,image_info.top_left(),image_info.bottom_right(),bg_col,tint_col);
}

bool RoundImageButton(const char* str_id,const char *image, const char *sub_image, const ImVec2 &size, const ImVec4& tint_col,float round)
{
    std::string image_key = image;
    if(sub_image)
        image_key = image_key + ":" + sub_image;
    auto image_info = TextureHolder::get(image_key);
    auto cur_pos = ImGui::GetCursorPos();
    auto min = cur_pos + ImGui::GetCurrentWindow()->Pos;
    auto max = min + size;
    bool clicked = ImGui::InvisibleButton(str_id,size);
    ImGui::SetCursorPos(cur_pos);
    bool hoverd = ImGui::IsItemHovered();
    bool actived = ImGui::IsItemActive();
    ImVec4 color_hover = tint_col;color_hover.w *= 0.8;
    ImVec4 color = actived ? tint_col : (hoverd ? color_hover : tint_col);
    ImGui::GetWindowDrawList()->AddImageRounded(image_info.texture,min,max,image_info.top_left(),image_info.bottom_right(),ImGui::ColorConvertFloat4ToU32(color),round);
    return clicked;
}

void RectangleFrame(const ImVec2 &size, ImU32 color,float round,float width)
{
    auto cur_pos = ImGui::GetCursorPos();
    auto min = cur_pos + ImGui::GetCurrentWindow()->Pos;
    auto max = min + size;
    ImGui::GetWindowDrawList()->AddRect(min,max,color,round,0,width);
}

void Rectangle(const ImVec2 &size, ImU32 color, float round)
{
    auto cur_pos = ImGui::GetCursorPos();
    auto min = cur_pos + ImGui::GetCurrentWindow()->Pos;
    auto max = min + size;
    ImGui::GetWindowDrawList()->AddRectFilled(min,max,color,round,0);
}

#define kDragDeltaSumSize 3
void DragScrollCurrentWindow(bool& draged, int mouse_button, float release_speed, float decelerate_factor, bool scroll_y, bool scroll_x)
{
    ImGuiContext& g = *ImGui::GetCurrentContext();
    ImGuiWindow* window = g.CurrentWindow;

    bool hovered = false;
    bool held = false;
    ImGuiButtonFlags button_flags = (mouse_button == 0) ? ImGuiButtonFlags_MouseButtonLeft : (mouse_button == 1) ? ImGuiButtonFlags_MouseButtonRight : ImGuiButtonFlags_MouseButtonMiddle;
    if (g.HoveredId == 0) // If nothing hovered so far in the frame (not same as IsAnyItemHovered()!)
        ImGui::ButtonBehavior(window->Rect(), window->GetID("##scrolldraggingoverlay"), &hovered, &held, button_flags);

    held = ImGui::IsWindowFocused();
    if(!held)
        return;
    held &= ImGui::IsMouseHoveringRect(window->Rect().GetTL(),window->Rect().GetBR());
    bool mouse_down = ImGui::GetIO().MouseDown[mouse_button];
    bool held_current = held & mouse_down;

    struct DragInfo{
        bool draged = false;
        float last_delta_x = 0;
        float last_delta_y = 0;

        float delta_x_sum_[kDragDeltaSumSize] = {0};
        float delta_y_sum_[kDragDeltaSumSize] = {0};
        int delta_x_sum_index_ = 0;
        int delta_y_sum_index_ = 0;
    };

    static std::map<ImGuiID,DragInfo> window_drag_info;
    if(window_drag_info.find(window->ID) == window_drag_info.end()){
        window_drag_info[window->ID] = DragInfo();
    }
    DragInfo& drag_info = window_drag_info[window->ID];

    ImVec2 mouse_delta = ImGui::GetIO().MouseDelta;
    mouse_delta.x *= -1;
    mouse_delta.y *= -1;

    if (held_current && scroll_x && mouse_delta.x != 0.0f){
        ImGui::SetScrollX(window, window->Scroll.x + mouse_delta.x);
        if(drag_info.draged == false && abs(mouse_delta.x) > 2)
            drag_info.draged = true;
    }
    if (held_current && scroll_y && mouse_delta.y != 0.0f){
        ImGui::SetScrollY(window, window->Scroll.y + mouse_delta.y);
        if(drag_info.draged == false && abs(mouse_delta.y) > 2)
            drag_info.draged = true;
    }

    drag_info.delta_x_sum_[(drag_info.delta_x_sum_index_++)%kDragDeltaSumSize] = mouse_delta.x;
    drag_info.delta_y_sum_[(drag_info.delta_y_sum_index_++)%kDragDeltaSumSize] = mouse_delta.y;

    if(ImGui::GetIO().MouseReleased[mouse_button] && held){
        drag_info.draged = false;
        float sum = 0;
        ARRARY_SUM_CLEAR(drag_info.delta_x_sum_,kDragDeltaSumSize,sum);
        drag_info.last_delta_x = sum / kDragDeltaSumSize * release_speed;

        sum = 0;
        ARRARY_SUM_CLEAR(drag_info.delta_y_sum_,kDragDeltaSumSize,sum);
        drag_info.last_delta_y = sum / kDragDeltaSumSize * release_speed;
    }

    if(drag_info.last_delta_y != 0){
        drag_info.last_delta_y *= decelerate_factor;
        if(abs(drag_info.last_delta_y) < 1)
            drag_info.last_delta_y = 0;
        ImGui::SetScrollY(window, window->Scroll.y + drag_info.last_delta_y);

        if(window->Scroll.y == 0 || window->Scroll.y >= window->ScrollMax.y || mouse_down){
            drag_info.last_delta_y = 0;
        }
    }

    if(drag_info.last_delta_x != 0){
        drag_info.last_delta_x *= decelerate_factor;
        if(abs(drag_info.last_delta_x) < 1)
            drag_info.last_delta_x = 0;
        ImGui::SetScrollX(window, window->Scroll.x + drag_info.last_delta_x);
        if(window->Scroll.x == 0 || window->Scroll.x >= window->ScrollMax.x || mouse_down){
            drag_info.last_delta_x = 0;
        }
    }
    draged = drag_info.draged;
}

int g_rotation_start_index;
void ImRotateStart()
{
    g_rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = g_rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x+u.x)/2, (l.y+u.y)/2); // or use _ClipRectStack?
}

void ImRotateEnd(float rad, ImVec2 center)
{
    float s=sin(rad), c=cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = g_rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void TextAligined(const char *str, const ImVec2 &size, int aligin_h, int aligin_v)
{
    ImVec2 text_size = ImGui::CalcTextSize(str);
    int x = 0;
    int y = 0;
    if(aligin_h == 1)
        x = (size.x - text_size.x) / 2;
    else if(aligin_h == 2)
        x = size.x - text_size.x;

    if(aligin_v == 1)
        y = (size.y - text_size.y) / 2;
    else if(aligin_v == 2)
        y = size.y - text_size.y;

    auto cur_pos = ImGui::GetCursorPos();
    cur_pos += ImVec2(x,y);
    ImGui::SetCursorPos(cur_pos);
    ImGui::Text("%s", str);
}




#endif

}//namespace mountain-ripper
