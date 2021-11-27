#include "GaussianNoise.h"
#include "Visioneer/Core/Base.h"

#include <imgui.h>

namespace Visioneer
{

GaussianNoise::GaussianNoise()
{
    VSR_INFO("Begin");
    VSR_INFO("End");
}

GaussianNoise::~GaussianNoise()
{
    VSR_INFO("Begin");
    VSR_INFO("End");
}

void GaussianNoise::augmentImage(cv::Mat image) const
{
    // https://answers.opencv.org/question/36309/opencv-gaussian-noise/
    cv::Mat tmp, noise = cv::Mat(image.size(), CV_32FC3);
    cv::randn(noise, mMean, mStd);
    image.convertTo(tmp, CV_32F, 1. / 255.);
    tmp += noise;
    cv::normalize(tmp, image, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void GaussianNoise::onImGuiRender()
{
    mIsUpdated = false;

    if (ImGui::TreeNode(toString(getType()).c_str()))
    {
        mIsUpdated |= ImGui::SliderFloat("##Mean", &mMean, -1.f, 1.f, "Mean = %.2f");
        mIsUpdated |= ImGui::SliderFloat("##Std", &mStd, 0.f, 3.f, "Std = %.2f");

        ImGui::TreePop();
    }
}

}
