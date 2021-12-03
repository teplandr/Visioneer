#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace Visioneer
{

ContentBrowserPanel::ContentBrowserPanel()
    : mCurrentDirectory(std::filesystem::current_path().parent_path().append("resources").append("images"))
{
}

void ContentBrowserPanel::onImGuiRender()
{
    ImGui::Begin("Content Browser");

    ImGui::TextWrapped("WD: %s", mCurrentDirectory.c_str());

    if (ImGui::Selectable(".."))
        mCurrentDirectory = mCurrentDirectory.parent_path();

    for (const auto& directoryEntry : std::filesystem::directory_iterator(mCurrentDirectory))
    {
        const auto& path = directoryEntry.path();
        std::string filenameString = path.filename().string();

        std::string label = directoryEntry.is_directory() ? "/" + filenameString : filenameString;
        if (ImGui::Selectable(label.c_str()))
        {
            if (directoryEntry.is_directory())
                mCurrentDirectory /= path.filename();
        }

        if (ImGui::BeginDragDropSource())
        {
            const auto& filePath = path.string();
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", filePath.c_str(), filePath.size());
            ImGui::EndDragDropSource();
        }
    }

    ImGui::End();
}

}
