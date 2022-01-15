#include "Visioneer/Models/PSPNet.h"

#include <set>
#include <numeric>

#include <opencv2/opencv.hpp>

#include "Visioneer/Core/Base.h"

namespace Visioneer
{

PSPNet::PSPNet()
{
}

PSPNet::~PSPNet()
{
}

void PSPNet::onAttach()
{
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(4);
    sessionOptions.SetInterOpNumThreads(4);
    sessionOptions.SetGraphOptimizationLevel(ORT_ENABLE_ALL);

    mSession = Ort::Session(mEnv, "../resources/models/PSPNet_PascalVOC.onnx", sessionOptions);

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

    mIsAttached = true;
}

void PSPNet::onDetach()
{
    VSR_INFO("");

    mInputDims.clear();
    mOutputDims.clear();
    mInputNames.clear();
    mOutputNames.clear();

    mIsAttached = false;
}

VariantAnnotation PSPNet::forward(const cv::Mat& image)
{
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);

    cv::Mat inputImageAsTensor = cv::dnn::blobFromImage(image, 1 / 57.5, cv::Size(mInputDims.at(3), mInputDims.at(2)),
                                                        /*mean*/ {123.675, 116.28, 103.53}, /*swapRB*/ true, /*crop*/ false, CV_32F);
    std::vector<Ort::Value> inputTensors;
    inputTensors.push_back(Ort::Value::CreateTensor<float>(memoryInfo,
                                                           inputImageAsTensor.ptr<float>(),
                                                           std::accumulate(mInputDims.begin(), mInputDims.end(), 1, std::multiplies<int64_t>()),
                                                           mInputDims.data(),
                                                           mInputDims.size()));

    std::vector<int64_t> outputTensorValues(std::accumulate(mOutputDims.begin(), mOutputDims.end(), 1, std::multiplies<int64_t>()));

    std::vector<Ort::Value> outputTensors;
    outputTensors.push_back(Ort::Value::CreateTensor<int64_t>(memoryInfo,
                                                              outputTensorValues.data(),
                                                              outputTensorValues.size(),
                                                              mOutputDims.data(),
                                                              mOutputDims.size()));

    mSession.Run(Ort::RunOptions{}, mInputNames.data(), inputTensors.data(), inputTensors.size(),
                                    mOutputNames.data(), outputTensors.data(), outputTensors.size());

    std::set<int64_t> uniqueClassIDs;
    int64_t outputCols = mOutputDims.at(3);
    int64_t outputRows = mOutputDims.at(2);
    cv::Mat mask(outputRows, outputCols, CV_8UC1);
    for (int row = 0; row < outputRows; ++row)
    {
        for (int col = 0; col < outputCols; ++col)
        {
            int index = col + row * outputCols;
            uniqueClassIDs.emplace(outputTensorValues[index]);
            mask.at<uint8_t>(row, col) = uint8_t(outputTensorValues[index]);
        }
    }
    VSR_INFO("Image has following classes: {}", fmt::join(uniqueClassIDs, ", "));

    SemanticSegmentAnnotation annotation;
    annotation.Mask = mask;
    return annotation;

    // another option - cv::findContours() and cv::approxPolyDP()
    // pros - more memory efficient
    // cons - less precise on mask's edges
    // https://docs.opencv.org/4.x/dd/d49/tutorial_py_contour_features.html
}

}
