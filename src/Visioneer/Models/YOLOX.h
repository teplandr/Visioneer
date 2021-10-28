#pragma once

#include "Visioneer/Models/Model.h"

#include <onnxruntime_cxx_api.h>

namespace Visioneer
{

class YOLOX : public Model
{
public:
    YOLOX();
    ~YOLOX() override;

    void onAttach() override;
    void onDetach() override;

    VariantAnnotation forward(const cv::Mat &image) override;

    std::string getName() const override { return "YOLOX"; }

private:
    Ort::Env mEnv{OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "YOLOX"};
    Ort::Session mSession{nullptr};
};

}
