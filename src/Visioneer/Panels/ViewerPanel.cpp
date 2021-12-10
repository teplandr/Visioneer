#include "ViewerPanel.h"
#include "ImageAugmentationPanel.h"

// temporary
#include <opencv2/imgcodecs.hpp>

#include "Visioneer/Core/Base.h"

namespace Visioneer
{

ViewerPanel::ViewerPanel()
    : mTexture(std::make_shared<Texture>(512, 512))
{
}

void ViewerPanel::onImGuiRender()
{
    ImGui::Begin("Viewer", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

    ImVec2 displayTextureSize = ImVec2(mTexture->width(), mTexture->height());

    // Image size setup
    float sourceWidth  = mTexture->width();
    float sourceHeight = mTexture->height();
    float availableWidth  = ImGui::GetContentRegionAvail().x;
    float availableHeight = ImGui::GetContentRegionAvail().y;
    if (sourceWidth > availableWidth || sourceHeight > availableHeight)
    {
        float aspectRatio = sourceWidth / sourceHeight;
        displayTextureSize = (aspectRatio >= 1.) ? ImVec2(availableWidth, availableWidth / aspectRatio) :
                                                   ImVec2(availableHeight * aspectRatio, availableHeight);
    }

    float xOffset = std::max(0.f, (availableWidth  - displayTextureSize.x) / 2.f);
    float yOffset = std::max(0.f, (availableHeight - displayTextureSize.y) / 2.f);
    ImVec2 newCursorPos(ImGui::GetCursorPos().x + xOffset, ImGui::GetCursorPos().y + yOffset);
    ImGui::SetCursorPos(newCursorPos);

    mRenderer.setInitPos(ImGui::GetCursorScreenPos(), displayTextureSize);

    ImGui::Image(reinterpret_cast<void*>(mTexture->rendererID()), displayTextureSize);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            const char* filePath = reinterpret_cast<const char*>(payload->Data);
            int size = payload->DataSize;

            auto image = cv::imread(std::string(filePath, static_cast<size_t>(size)));
            mTexture = std::make_shared<Texture>(image);

            if (mListener)
                mListener->onImageLoad(image);
        }
        ImGui::EndDragDropTarget();
    }

    ImVec2 mousePos = ImGui::IsItemHovered() ? ImGui::GetMousePos() : ImVec2();
    mRenderer.setMousePos(mousePos);

    std::visit(mRenderer, mAnnotation);

    ImGui::End();
}

void ViewerPanel::onImageUpdate(const cv::Mat &image)
{
    VSR_TRACE("");

    mTexture->setData(image);
}

void ViewerPanel::onAnnotationUpdate(const VariantAnnotation &annotation)
{
    VSR_TRACE("");

    mAnnotation = annotation;
}

}
