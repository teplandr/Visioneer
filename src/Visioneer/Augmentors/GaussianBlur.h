#pragma once

#include "Visioneer/Augmentors/Augmentor.h"

namespace Visioneer
{

class GaussianBlur : public Augmentor
{
public:
    GaussianBlur();
    ~GaussianBlur() override;

    void augmentImage(cv::Mat image) const override;
    void onImGuiRender() override;
    Type getType() const override { return Type::GaussianBlur; }

private:
    int mKernelSize = 0;
    float mSigma = 1.f;
};

}
