#pragma once

#include <opencv2/dnn.hpp>

#include "Visioneer/Models/Model.h"

namespace Visioneer
{

class UltraFace : public Model
{
public:
    UltraFace();
    ~UltraFace() override;

    void onAttach() override;
    void onDetach() override;

    VariantAnnotation forward(const cv::Mat &image) override;

    std::string getName() const override { return "UltraFace"; }

    // temporary public (I guess)
    float ConfidenceThreshold = 0.5f;
    float SupperessionThreshold = 0.5f;

private:
    cv::dnn::Net mNet;
};

}
