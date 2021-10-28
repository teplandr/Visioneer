#pragma once

#include <opencv2/core.hpp>

#include "Visioneer/Models/Model.h"

namespace Visioneer
{

class ViewerPanel;

class ModelsControlPanel
{
public:
    ModelsControlPanel();
    ~ModelsControlPanel();

    void onImGuiRender();
    void onImageUpdate(const cv::Mat& image);

    void setOnAnnotationUpdateListener(ViewerPanel *listener) { mListener = listener; }

private:
    void runSelectedModel();
    void drawSelectedModel();

    template<typename ModelType, typename DrawFuncPtr>
    bool drawModel(DrawFuncPtr drawFuncPtr);

private:
    ViewerPanel *mListener;

    cv::Mat mInputImage;

    std::vector<Model *> mModels;
    Model *mSelectedModel = nullptr;
};

}
