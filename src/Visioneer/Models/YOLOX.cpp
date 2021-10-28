#include "YOLOX.h"

#include "Visioneer/Core/Base.h"

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

    mIsAttached = true;
}

void YOLOX::onDetach()
{
    mIsAttached = false;
}

VariantAnnotation YOLOX::forward(const cv::Mat& image)
{
    Ort::AllocatorWithDefaultOptions allocator;

    VSR_INFO("Amount of Inputs: {}", mSession.GetInputCount());
    VSR_INFO("Amount of Outputs: {}", mSession.GetOutputCount());

    const char* inputName = mSession.GetInputName(0, allocator);
    VSR_INFO("Input Name: {}", inputName);

    Ort::TypeInfo inputTypeInfo = mSession.GetInputTypeInfo(0);
    auto inputTensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();

    ONNXTensorElementDataType inputType = inputTensorInfo.GetElementType();
    VSR_INFO("Input Type: {}", inputType);

    const std::vector<int64_t>& inputDims = inputTensorInfo.GetShape();
    VSR_INFO("Input Dimensions: {}", fmt::join(inputDims, "x"));

    const char* outputName = mSession.GetOutputName(0, allocator);
    VSR_INFO("Output Name: {}", outputName);

    Ort::TypeInfo outputTypeInfo = mSession.GetOutputTypeInfo(0);
    auto outputTensorInfo = outputTypeInfo.GetTensorTypeAndShapeInfo();

    ONNXTensorElementDataType outputType = outputTensorInfo.GetElementType();
    VSR_INFO("Output Type: {}", outputType);

    const std::vector<int64_t>& outputDims = outputTensorInfo.GetShape();
    VSR_INFO("Output Dimensions: {}", fmt::join(outputDims, "x"));

    cv::Mat inputImageAsTensor = cv::dnn::blobFromImage(image, 1., cv::Size(inputDims.at(3), inputDims.at(2)), {}, /*swapRB*/ true, /*crop*/ false, CV_32F);

    std::vector<float> outputTensorValues(outputTensorInfo.GetElementCount());

    std::vector<const char*> inputNames{inputName};
    std::vector<const char*> outputNames{outputName};
    std::vector<Ort::Value> inputTensors;
    std::vector<Ort::Value> outputTensors;

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
    inputTensors.push_back(Ort::Value::CreateTensor<float>(memoryInfo,
                                                           inputImageAsTensor.ptr<float>(),
                                                           inputTensorInfo.GetElementCount(),
                                                           inputDims.data(),
                                                           inputDims.size()));
    outputTensors.push_back(Ort::Value::CreateTensor<float>(memoryInfo,
                                                            outputTensorValues.data(),
                                                            outputTensorValues.size(),
                                                            outputDims.data(),
                                                            outputDims.size()));

    mSession.Run(Ort::RunOptions{nullptr}, inputNames.data(), inputTensors.data(), 1, outputNames.data(), outputTensors.data(), 1);

    /* Do not work
    BBoxesAnnotation annotation;
    annotation.BBoxes.reserve(128);
    annotation.Scores.reserve(128);
    annotation.Classes.reserve(128);

    std::vector<int> strides{8, 16, 32};
    for (size_t strideIndex = 0; strideIndex < strides.size(); ++strideIndex)
    {
        const int stride = strides[strideIndex];
        int numGridY = inputDims.at(2) / stride;
        int numGridX = inputDims.at(3) / stride;
        for (int gridY = 0; gridY < numGridY; ++gridY)
        {
            for (int gridX = 0; gridX < numGridX; ++gridX)
            {
                const int index = gridX + numGridX * gridY + numGridX * numGridY * strideIndex + numGridX * numGridY * (80 + 5);

                // yolox/models/yolo_head.py decode logic
                float xCenter = (outputTensorValues[index + 0] + gridX) * stride;
                float yCenter = (outputTensorValues[index + 1] + gridY) * stride;
                float w = exp(outputTensorValues[index + 2]) * stride;
                float h = exp(outputTensorValues[index + 3]) * stride;

                float x1 = xCenter - w * 0.5f;
                float y1 = yCenter - h * 0.5f;

                annotation.BBoxes.emplace_back(x1, y1, x1 + w, y1 + h);
                annotation.Scores.emplace_back(1.);
                annotation.Classes.emplace_back(0);
            }
        }
    }
    */

    /* For debug
    for (int64_t i = 0; i < outputDims.at(1); ++i)
    {
        if (outputTensorValues[outputDims.at(2) * i + 4] > 0.5f)
            VSR_INFO("{:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f}", outputTensorValues[outputDims.at(2) * i + 0],
                                                                  outputTensorValues[outputDims.at(2) * i + 1],
                                                                  outputTensorValues[outputDims.at(2) * i + 2],
                                                                  outputTensorValues[outputDims.at(2) * i + 3],
                                                                  outputTensorValues[outputDims.at(2) * i + 4],
                                                                  outputTensorValues[outputDims.at(2) * i + 5]);
    }
    */

    return EmptyAnnotation();
}

}
