#include "YOLOX.h"
#include "Visioneer/Core/Base.h"
#include "Visioneer/Models/Utilities.h"

#include <numeric>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

namespace Visioneer
{

YOLOX::YOLOX()
{
}

YOLOX::~YOLOX()
{
}

void YOLOX::onAttach()
{
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(4);
    sessionOptions.SetGraphOptimizationLevel(ORT_ENABLE_ALL);

    mSession = Ort::Session(mEnv, "../resources/models/YOLOX_Tiny.onnx", sessionOptions);

    Ort::AllocatorWithDefaultOptions allocator;

    VSR_INFO("Amount of Inputs: {}", mSession.GetInputCount());
    VSR_INFO("Amount of Outputs: {}", mSession.GetOutputCount());

    // for each input

        const char* inputName = mSession.GetInputName(0, allocator);
        VSR_INFO("Input Name: {}", inputName);
        mInputNames.emplace_back(inputName);

        Ort::TypeInfo inputTypeInfo = mSession.GetInputTypeInfo(0);
        auto inputTensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();

        ONNXTensorElementDataType inputType = inputTensorInfo.GetElementType();
        VSR_INFO("Input Type: {}", inputType);

        mInputDims = inputTensorInfo.GetShape();
        VSR_INFO("Input Dimensions: {}", fmt::join(mInputDims, "x"));

    // for each output

        const char* outputName = mSession.GetOutputName(0, allocator);
        VSR_INFO("Output Name: {}", outputName);
        mOutputNames.emplace_back(outputName);

        Ort::TypeInfo outputTypeInfo = mSession.GetOutputTypeInfo(0);
        auto outputTensorInfo = outputTypeInfo.GetTensorTypeAndShapeInfo();

        ONNXTensorElementDataType outputType = outputTensorInfo.GetElementType();
        VSR_INFO("Output Type: {}", outputType);

        mOutputDims = outputTensorInfo.GetShape();
        VSR_INFO("Output Dimensions: {}", fmt::join(mOutputDims, "x"));

    initMultipliers();

    mIsAttached = true;
}

void YOLOX::onDetach()
{
    VSR_INFO("");

    mInputDims.clear();
    mOutputDims.clear();
    mInputNames.clear();
    mOutputNames.clear();
    mMultipliers.clear();

    mIsAttached = false;
}

VariantAnnotation YOLOX::forward(const cv::Mat& image)
{
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);

    cv::Mat inputImageAsTensor = cv::dnn::blobFromImage(image, 1., cv::Size(mInputDims.at(3), mInputDims.at(2)),
                                                        /*mean*/ {}, /*swapRB*/ true, /*crop*/ false, CV_32F);
    std::vector<Ort::Value> inputTensors;
    inputTensors.push_back(Ort::Value::CreateTensor<float>(memoryInfo,
                                                           inputImageAsTensor.ptr<float>(),
                                                           std::accumulate(mInputDims.begin(), mInputDims.end(), 1, std::multiplies<int64_t>()),
                                                           mInputDims.data(),
                                                           mInputDims.size()));

    std::vector<float> outputTensorValues(std::accumulate(mOutputDims.begin(), mOutputDims.end(), 1, std::multiplies<int64_t>()));
    std::vector<Ort::Value> outputTensors;
    outputTensors.push_back(Ort::Value::CreateTensor<float>(memoryInfo,
                                                            outputTensorValues.data(),
                                                            outputTensorValues.size(),
                                                            mOutputDims.data(),
                                                            mOutputDims.size()));

    mSession.Run(Ort::RunOptions{}, mInputNames.data(), inputTensors.data(), inputTensors.size(),
                                    mOutputNames.data(), outputTensors.data(), inputTensors.size());

    BBoxesAnnotation annotation;
    annotation.Type = BBoxesAnnotation::DatasetType::COCO;
    annotation.Items.reserve(128);

    for (size_t i = 0; i < mMultipliers.size(); ++i)
    {
        const auto& [gridX, gridY, stride] = mMultipliers.at(i);
        int index = i * 85;

        // yolox/models/yolo_head.py decode logic
        float xCenter = (outputTensorValues[index + 0] + gridX) * stride / mInputDims.at(3);
        float yCenter = (outputTensorValues[index + 1] + gridY) * stride / mInputDims.at(2);
        float w = exp(outputTensorValues[index + 2]) * stride / mInputDims.at(3);
        float h = exp(outputTensorValues[index + 3]) * stride / mInputDims.at(2);

        float x1 = xCenter - w * 0.5f;
        float y1 = yCenter - h * 0.5f;

        float objectness = outputTensorValues[index + 4];
        for (int classID = 0; classID < 80; ++classID)
        {
            float score = objectness * outputTensorValues[index + 5 + classID];
            if (score > ConfidenceThreshold)
            {
                Rect bbox{x1, y1, x1 + w, y1 + h};
                annotation.Items.emplace_back(bbox, score, classID);
            }
        }
    }

    Utilities::nonMaximumSupperession(annotation.Items, SupperessionThreshold);

    // is needed for visualization while mouse is hovering over rect
    std::sort(annotation.Items.begin(), annotation.Items.end(), [](const auto& lhs, const auto& rhs)
    {
        return lhs.BBox.area() < rhs.BBox.area();
    });

    return annotation;
}

void YOLOX::initMultipliers()
{
    std::vector<int> strides{8, 16, 32};
    for (const int stride : strides)
    {
        int numGridY = mInputDims.at(2) / stride;
        int numGridX = mInputDims.at(3) / stride;
        for (int gridY = 0; gridY < numGridY; ++gridY)
        {
            for (int gridX = 0; gridX < numGridX; ++gridX)
            {
                mMultipliers.emplace_back(gridX, gridY, stride);
            }
        }
    }
}

}
