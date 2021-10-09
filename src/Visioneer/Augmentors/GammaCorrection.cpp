#include "GammaCorrection.h"
#include "Visioneer/Core/Base.h"

#include <imgui.h>

namespace Visioneer
{

GammaCorrection::GammaCorrection()
{
    VSR_INFO("Begin");
    VSR_INFO("End");
}

GammaCorrection::~GammaCorrection()
{
    VSR_INFO("Begin");
    VSR_INFO("End");
}

void GammaCorrection::augmentImage(cv::Mat image) const
{
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* ptr = lookUpTable.ptr();
    for(int i = 0; i < 256; ++i)
        ptr[i] = cv::saturate_cast<uchar>(powf(i / 255.f, mGamma) * 255.f);
    cv::LUT(image, lookUpTable, image);
}

void GammaCorrection::onImGuiRender()
{
    mIsUpdated = false;

    if (ImGui::TreeNode(toString(getType()).c_str()))
    {
        mIsUpdated |= ImGui::SliderFloat("##Gamma", &mGamma, 0.f, 10.f, "Gamma = %.2f");

        ImGui::TreePop();
    }
}

}
