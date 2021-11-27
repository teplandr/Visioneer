#include "GaussianBlur.h"
#include "Visioneer/Core/Base.h"

#include <imgui.h>
#include <opencv2/imgproc.hpp>

namespace Visioneer
{

GaussianBlur::GaussianBlur()
{
    VSR_INFO("Begin");
    VSR_INFO("End");
}

GaussianBlur::~GaussianBlur()
{
    VSR_INFO("Begin");
    VSR_INFO("End");
}

void GaussianBlur::augmentImage(cv::Mat image) const
{
    cv::GaussianBlur(image, image, {2 * mKernelSize + 1, 2 * mKernelSize + 1}, mSigma, mSigma);
}

void GaussianBlur::onImGuiRender()
{
    mIsUpdated = false;

    if (ImGui::TreeNode(toString(getType()).c_str()))
    {
        mIsUpdated |= ImGui::SliderInt("##KernelSize", &mKernelSize, 0, 50, "KernelSize = %d");
        mIsUpdated |= ImGui::SliderFloat("##Sigma", &mSigma, 0.01f, 50.f, "Sigma = %.2f");

        ImGui::TreePop();
    }
}

}
