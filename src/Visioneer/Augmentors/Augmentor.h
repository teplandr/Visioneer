#pragma once

#include <memory>

#include <opencv2/core.hpp>

namespace Visioneer
{

class Augmentor
{
public:
    virtual ~Augmentor();

    bool isUpdated() const { return mIsUpdated; }

    virtual void augmentImage(cv::Mat image) const = 0;
    virtual void onImGuiRender() = 0;

    enum class Type
    {
        GammaCorrection,
        GaussianBlur,
        GaussianNoise
    };

    virtual Type getType() const = 0;
    static std::string toString(Type type);

    static std::unique_ptr<Augmentor> create(Type type);

protected:
    bool mIsUpdated = false;
};

struct AugmentorDescriptor
{
    Augmentor::Type Type;
    std::string Name;
    bool IsCreated;
    bool IsSelected;

    AugmentorDescriptor(Augmentor::Type type)
        : Type(type), Name(Augmentor::toString(type)), IsCreated(false), IsSelected(false)
    {}
};

}
