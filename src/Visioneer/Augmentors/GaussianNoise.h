#pragma once

#include "Visioneer/Augmentors/Augmentor.h"

namespace Visioneer
{

class GaussianNoise : public Augmentor
{
public:
    GaussianNoise();
    ~GaussianNoise() override;

    void augmentImage(cv::Mat image) const override;
    void onImGuiRender() override;
    Type getType() const override { return Type::GaussianNoise; }

private:
    float mMean = 0.f;
    float mStd  = 0.f;
};

}
