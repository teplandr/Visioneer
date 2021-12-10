#include "ImageAugmentationPanel.h"

#include "ViewerPanel.h"
#include "ModelsControlPanel.h"

#include <imgui.h>

#include "Visioneer/Core/Base.h"

namespace Visioneer
{

ImageAugmentationPanel::ImageAugmentationPanel()
{
    mAugmentorDescriptors.emplace_back(Augmentor::Type::GammaCorrection);
    mAugmentorDescriptors.emplace_back(Augmentor::Type::GaussianBlur);
    mAugmentorDescriptors.emplace_back(Augmentor::Type::GaussianNoise);
}

void ImageAugmentationPanel::onImGuiRender()
{
    ImGui::Begin("Image Augmentation Panel", nullptr, ImGuiWindowFlags_NoMove);

    if (ImGui::Button("Add/remove augmentation"))
        ImGui::OpenPopup("AugmentationPopup");

    bool isAugmentorsNumberChanged = false;
    if (ImGui::BeginPopup("AugmentationPopup"))
    {
        for (auto& augmentorDescriptor : mAugmentorDescriptors)
        {
            ImGui::MenuItem(augmentorDescriptor.Name.c_str(), "", &augmentorDescriptor.IsSelected);

            if (augmentorDescriptor.IsSelected && !augmentorDescriptor.IsCreated)
            {
                mAugmentors.emplace(augmentorDescriptor.Type, Augmentor::create(augmentorDescriptor.Type));
                augmentorDescriptor.IsCreated = true;
                isAugmentorsNumberChanged = true;
            }

            if (!augmentorDescriptor.IsSelected && augmentorDescriptor.IsCreated)
            {
                mAugmentors.erase(augmentorDescriptor.Type);
                augmentorDescriptor.IsCreated = false;
                isAugmentorsNumberChanged = true;
            }
        }
        ImGui::EndPopup();
    }

    for (const auto& [type, augmentor] : mAugmentors)
        augmentor->onImGuiRender();

    bool isAnyAugmentorUpdated = std::any_of(mAugmentors.begin(), mAugmentors.end(), [](const auto& typeAugmentorPair)
    {
        return typeAugmentorPair.second->isUpdated();
    });

    if (isAugmentorsNumberChanged || isAnyAugmentorUpdated)
        augmentImage();

    ImGui::End();
}

void ImageAugmentationPanel::onImageLoad(const cv::Mat &image)
{
    VSR_TRACE("");

    mSourceImage = image.clone();
    augmentImage();
}

void ImageAugmentationPanel::augmentImage()
{
    if (mSourceImage.empty())
        return;

    mAugmentedImage = mSourceImage.clone();

    for (const auto& [type, augmentor] : mAugmentors)
        augmentor->augmentImage(mAugmentedImage);

    if (mListener1)
        mListener1->onImageUpdate(mAugmentedImage);

    if (mListener2)
        mListener2->onImageUpdate(mAugmentedImage);
}

}
