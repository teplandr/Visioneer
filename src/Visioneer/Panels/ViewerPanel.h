#pragma once

#include <memory>

#include <opencv2/core.hpp>

#include "Visioneer/Core/Texture.h"
#include "Visioneer/Core/Annotation.h"
#include "Visioneer/Core/AnnotationRenderer.h"

#include <imgui.h>

namespace Visioneer
{

class ImageAugmentationPanel;

class ViewerPanel
{
public:
    ViewerPanel();

    void onImGuiRender();
    void onImageUpdate(const cv::Mat& image);
    void onAnnotationUpdate(const VariantAnnotation& annotation);

    void setOnImageLoadListener(ImageAugmentationPanel *listener) { mListener = listener; }

private:
    std::shared_ptr<Texture> mTexture;

    VariantAnnotation mAnnotation;
    AnnotationRenderer mRenderer;

    ImageAugmentationPanel *mListener;
};

}
