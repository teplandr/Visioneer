#include "UltraFace.h"

#include "Visioneer/Core/Base.h"
#include "Visioneer/Models/Utilities.h"

namespace Visioneer
{

UltraFace::UltraFace()
{
}

UltraFace::~UltraFace()
{
}

void UltraFace::onAttach()
{
    VSR_INFO("");

    // try catch
    mNet = cv::dnn::readNetFromONNX("../resources/models/UltraFace.onnx");
    mIsAttached = true;
}

void UltraFace::onDetach()
{
    VSR_INFO("");

    // somehow release mNet if possible
    mIsAttached = false;
}

VariantAnnotation UltraFace::forward(const cv::Mat &image)
{
    auto blob = cv::dnn::blobFromImage(image, 1. / 128., {640, 480}, {127., 127., 127.}, /*swapRB*/ true, /*crop*/ false, CV_32F);
    mNet.setInput(blob);

    int N = blob.size[0];
    int C = blob.size[1];
    int H = blob.size[2];
    int W = blob.size[3];

    VSR_TRACE("Blob shape: {}x{}x{}x{}", N, C, H, W);

    std::vector<cv::Mat> outputTensors;
    const auto& outputTensorNames = mNet.getUnconnectedOutLayersNames();
    mNet.forward(outputTensors, outputTensorNames);

    assert(outputTensors.size() == outputTensorNames.size());
    for (uint32_t i = 0; i < outputTensors.size(); ++i)
    {
        const auto& currentSize = outputTensors[i].size;
        assert(currentSize.dims() == 3);
        VSR_TRACE("{}: {}x{}x{}", outputTensorNames[i], currentSize[0], currentSize[1], currentSize[2]);
    }

    BBoxesAnnotation annotation;
    annotation.Type = BBoxesAnnotation::DatasetType::OneClass;
    annotation.Items.reserve(128);

    for (int i = 0; i < outputTensors[0].size[1]; ++i)
    {
        float currentConfidence = outputTensors[0].at<float>(0, i, 1);
        if (currentConfidence > ConfidenceThreshold)
        {
            Rect bbox {outputTensors[1].at<float>(0, i, 0),
                       outputTensors[1].at<float>(0, i, 1),
                       outputTensors[1].at<float>(0, i, 2),
                       outputTensors[1].at<float>(0, i, 3)};
            annotation.Items.emplace_back(bbox, currentConfidence, 0);
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

}
