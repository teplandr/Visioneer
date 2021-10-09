#include "Augmentor.h"

#include "GammaCorrection.h"

namespace Visioneer
{

Augmentor::~Augmentor()
{
}

std::string Augmentor::toString(Augmentor::Type type)
{
    switch(type)
    {
        case Type::GammaCorrection: return "Gamma Correction";
        case Type::GaussianBlur:    return "Gaussian Blur";
        case Type::GaussianNoise:   return "Gaussian Noise";
    }
    return {};
}

std::unique_ptr<Augmentor> Augmentor::create(Augmentor::Type type)
{
    switch(type)
    {
        case Type::GammaCorrection: return std::make_unique<GammaCorrection>();
//        case Type::GaussianBlur:    return "Gaussian Blur";
//        case Type::GaussianNoise:   return "Gaussian Noise";
    }

    return nullptr;
}

}
