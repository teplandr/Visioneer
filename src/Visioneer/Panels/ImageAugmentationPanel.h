#pragma once

#include <opencv2/core.hpp>

#include "Visioneer/Augmentors/Augmentor.h"

namespace Visioneer
{

class ViewerPanel;
class ModelsControlPanel;

class ImageAugmentationPanel
{
public:
    ImageAugmentationPanel();

    void onImGuiRender();
    void onImageLoad(const cv::Mat& image);

    void setOnImageUpdateListener1(ViewerPanel *listener) { mListener1 = listener; }
    void setOnImageUpdateListener2(ModelsControlPanel *listener) { mListener2 = listener; }

private:
    void augmentImage();

private:
    cv::Mat mSourceImage, mAugmentedImage;

    ViewerPanel *mListener1;
    ModelsControlPanel *mListener2;

    std::vector<AugmentorDescriptor> mAugmentorDescriptors;
    std::unordered_map<Augmentor::Type, std::unique_ptr<Augmentor>> mAugmentors;
};

}
