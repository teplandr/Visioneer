#include "Visioneer/ImGui/CustomWidgets.h"

#include <imgui.h>

namespace Visioneer
{

void ToggleButton(const char* label, bool* state)
{
    // adapted from https://github.com/ocornut/imgui/issues/1537#issuecomment-780262461

    float height = ImGui::GetFrameHeight();
    float width  = height * 1.55f;
    float radius = height * 0.50f;

    ImVec2 tl = ImGui::GetCursorScreenPos();
    ImVec2 br(tl.x + width, tl.y + height);
    ImVec2 circlePosition(tl.x + radius + float(*state) * (width - radius * 2.f), tl.y + radius);

    ImGui::InvisibleButton(label, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *state = !*state;

    ImVec4* colors = ImGui::GetStyle().Colors;
    ImU32 hoveredColor   = ImGui::GetColorU32(*state ? colors[ImGuiCol_TabHovered] : colors[ImGuiCol_FrameBgHovered]);
    ImU32 unhoveredColor = ImGui::GetColorU32(*state ? colors[ImGuiCol_TabActive]  : colors[ImGuiCol_Border]);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(tl, br, ImGui::IsItemHovered() ? hoveredColor : unhoveredColor, radius);
    drawList->AddCircleFilled(circlePosition, radius - 1.5f, ImGui::GetColorU32(colors[ImGuiCol_SliderGrabActive]));
}

}
