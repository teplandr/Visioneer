#pragma once

#include <opencv2/dnn.hpp>

#include "Visioneer/Core/Annotation.h"

namespace Visioneer
{

class Model
{
public:
    virtual ~Model() = default;

    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual std::string getName() const = 0;
    virtual bool isAttached() const { return mIsAttached; }
    virtual VariantAnnotation forward(const cv::Mat& image) = 0;

protected:
    bool mIsAttached = false;
};


class UltraFace : public Model
{
public:
    UltraFace();
    ~UltraFace() override;

    void onAttach() override;
    void onDetach() override;

    std::string getName() const override { return "UltraFace"; }

    VariantAnnotation forward(const cv::Mat &image) override;

    float ConfidenceThreshold = 0.5f;

private:
    cv::dnn::Net mNet;

};

class OpenPose : public Model
{
public:
    OpenPose() {}
    ~OpenPose() override {}

    void onAttach() override { mIsAttached = true;  }
    void onDetach() override { mIsAttached = false; }

    std::string getName() const override { return "OpenPose"; }

    VariantAnnotation forward(const cv::Mat &image) override { return {}; }
};

}
