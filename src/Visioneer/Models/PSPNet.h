#pragma once

#include "Visioneer/Models/Model.h"

#include <onnxruntime_cxx_api.h>

namespace Visioneer
{

class PSPNet : public Model
{
public:
    PSPNet();
    ~PSPNet() override;

    void onAttach() override;
    void onDetach() override;

    VariantAnnotation forward(const cv::Mat &image) override;

    std::string getName() const override { return "PSPNet"; }

private:
    Ort::Env mEnv{OrtLoggingLevel::ORT_LOGGING_LEVEL_VERBOSE, "PSPNet"};
    Ort::Session mSession{nullptr};

    std::vector<int64_t> mInputDims;
    std::vector<int64_t> mOutputDims;
    std::vector<const char*> mInputNames;
    std::vector<const char*> mOutputNames;
};

}
