#include "Visioneer/ImGui/CustomWidgets.h"

#include <imgui.h>

namespace Visioneer
{

void ToggleButton(const char* label, bool* state)
{
    // taken from https://github.com/ocornut/imgui/issues/1537#issuecomment-780262461

    ImVec4* colors = ImGui::GetStyle().Colors;
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(label, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *state = !*state;
    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), ImGui::GetColorU32(*state ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.f)), height * 0.5f);
    else
        draw_list->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), ImGui::GetColorU32(*state ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.f)), height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(pos.x + radius + (*state ? 1 : 0) * (width - radius * 2.f), pos.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

}
