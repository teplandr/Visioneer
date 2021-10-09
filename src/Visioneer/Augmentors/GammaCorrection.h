#pragma once

#include "Visioneer/Augmentors/Augmentor.h"

namespace Visioneer
{

class GammaCorrection : public Augmentor
{
public:
    GammaCorrection();
    ~GammaCorrection() override;

    void augmentImage(cv::Mat image) const override;
    void onImGuiRender() override;
    Type getType() const override { return Type::GammaCorrection; }

private:
    float mGamma = 1.f;
};

}
