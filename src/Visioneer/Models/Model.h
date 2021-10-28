#pragma once

#include <opencv2/core.hpp>

#include "Visioneer/Core/Annotation.h"

namespace Visioneer
{

class Model
{
public:
    virtual ~Model() = default;

    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual VariantAnnotation forward(const cv::Mat& image) = 0;

    virtual std::string getName() const = 0;
    virtual bool isAttached() const { return mIsAttached; }

protected:
    bool mIsAttached = false;
};

}
