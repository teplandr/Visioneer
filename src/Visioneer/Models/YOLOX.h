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

    std::vector<const char*> getVersionNames() const { return mVersionNames; }

    // temporary public (I guess)
    float ConfidenceThreshold = 0.5f;
    float SupperessionThreshold = 0.5f;
    int CurrentVersionNameIndex = 0;

private:
    void initMultipliers();

private:
    Ort::Env mEnv{OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "YOLOX"};
    Ort::Session mSession{nullptr};

    std::vector<int64_t> mInputDims;
    std::vector<int64_t> mOutputDims;
    std::vector<const char*> mInputNames;
    std::vector<const char*> mOutputNames;

    std::vector<std::tuple<int, int, int>> mMultipliers;

    struct VersionNameModelPath
    {
        const char* Name;
        const char* Path;
    };

    const std::vector<VersionNameModelPath> mVersionNamesModelPaths
    {
        {"Tiny",   "../resources/models/YOLOX_Tiny.onnx"},
        {"Medium", "../resources/models/YOLOX_M.onnx"}
    };

    std::vector<const char*> mVersionNames;
};

}
