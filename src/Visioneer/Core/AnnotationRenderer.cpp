#include "AnnotationRenderer.h"

#include <unordered_map>

namespace Visioneer
{

static std::vector<const char*> ClassNamesCOCO
{
    "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
    "hair drier", "toothbrush"
};

static std::vector<uint32_t> ColorsCOCO
{
    0xff3c14dc, 0xffa9a9a9, 0xffdcdcdc, 0xff4f4f2f, 0xff2f6b55, 0xff13458b, 0xff238e6b, 0xff578b2e,
    0xff228b22, 0xff00007f, 0xff701919, 0xff006400, 0xff008080, 0xff8b3d48, 0xff2222b2, 0xffa09e5f,
    0xff998877, 0xff71b33c, 0xff8f8fbc, 0xff993366, 0xff808000, 0xff6bb7bd, 0xff3f85cd, 0xffb48246,
    0xff1e69d2, 0xff32cd9a, 0xffaab220, 0xff5c5ccd, 0xff8b0000, 0xff82004b, 0xff32cd32, 0xff20a5da,
    0xff8fbc8f, 0xff800080, 0xff6030b0, 0xffaacd66, 0xffcc3299, 0xff0045ff, 0xff008cff, 0xff00a5ff,
    0xff00d7ff, 0xff00ffff, 0xff8515c7, 0xffcd0000, 0xff87b8de, 0xffd0e040, 0xff00ff7f, 0xff00ff00,
    0xffd355ba, 0xff9afa00, 0xffe22b8a, 0xff7fff00, 0xffe16941, 0xff7a96e9, 0xff696969, 0xffffff00,
    0xffffbf00, 0xffdb7093, 0xffff0000, 0xff2fffad, 0xffd670da, 0xffd8bfd8, 0xffdec4b0, 0xff507fff,
    0xffff00ff, 0xff9370db, 0xff7280fa, 0xffaae8ee, 0xff54ffff, 0xffed9564, 0xffdda0dd, 0xffe6e0b0,
    0xff90ee90, 0xff9314ff, 0xffee687b, 0xffface87, 0xffd4ff7f, 0xffb9daff, 0xffb469ff, 0xffc1b6ff
};

static std::unordered_map<BBoxesAnnotation::DatasetType, std::vector<const char*>> TypeSpecificClassNames
{
    {BBoxesAnnotation::DatasetType::OneClass, std::vector<const char*>{""}},
    {BBoxesAnnotation::DatasetType::COCO, ClassNamesCOCO}
};

static std::unordered_map<BBoxesAnnotation::DatasetType, std::vector<uint32_t>> TypeSpecificColors
{
    {BBoxesAnnotation::DatasetType::OneClass, std::vector<uint32_t>{0xff3c14dc}},
    {BBoxesAnnotation::DatasetType::COCO, ColorsCOCO}
};

static std::vector<const char*> ClassNamesVOC
{
    "background", "aeroplane", "bicycle", "bird", "boat", "bottle",
    "bus", "car", "cat", "chair", "cow", "diningtable", "dog",
    "horse", "motorbike", "person", "pottedplant", "sheep", "sofa",
    "train", "tvmonitor"
};

static std::vector<uint32_t> ColorsVOC
{
    0xff000000, 0xff3c14dc, 0xffdcdcdc, 0xff4f4f2f, 0xff2f6b55, 0xff13458b, 0xff238e6b, 0xff578b2e,
    0xff228b22, 0xff00007f, 0xff701919, 0xff006400, 0xff008080, 0xff8b3d48, 0xff2222b2, 0xffa09e5f,
    0xff998877, 0xff71b33c, 0xff8f8fbc, 0xff993366, 0xffa9a9a9
};

void AnnotationRenderer::operator()(const EmptyAnnotation&)
{
}

void AnnotationRenderer::operator()(const BBoxesAnnotation& annotation)
{
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    bool isBBoxSelected = false;
    for (const auto& [bbox, score, classID] : annotation.Items)
    {
        ImVec2 tl(mInitPos.x + bbox.X1 * mViewerSize.x, mInitPos.y + bbox.Y1 * mViewerSize.y);
        ImVec2 br(mInitPos.x + bbox.X2 * mViewerSize.x, mInitPos.y + bbox.Y2 * mViewerSize.y);
        drawList->AddRect(tl, br, TypeSpecificColors.at(annotation.Type).at(classID), 0.f, 0, 3.f);

        if (!isBBoxSelected && mMousePos.x >= tl.x && mMousePos.x <= br.x && mMousePos.y >= tl.y && mMousePos.y <= br.y)
        {
            drawList->AddRectFilled(tl, br, changeAlpha(TypeSpecificColors.at(annotation.Type).at(classID), 0.25f), 0.f, 0);
            ImGui::SetTooltip("%s (%.3f)", TypeSpecificClassNames.at(annotation.Type).at(classID), score);
            isBBoxSelected = true;
        }
    }
}

void AnnotationRenderer::operator()(const SemanticSegmentAnnotation& annotation)
{
    // In future, this transform should be done by shader

    uint8_t selectedClassID = 255;
    if (mMousePos.x > 0.f && mMousePos.y > 0.f)
    {
        int hoveredRow = static_cast<int>(annotation.Mask.rows * (mMousePos.y - mInitPos.y) / mViewerSize.y);
        int hoveredCol = static_cast<int>(annotation.Mask.cols * (mMousePos.x - mInitPos.x) / mViewerSize.x);

        selectedClassID = annotation.Mask.at<uint8_t>(hoveredRow, hoveredCol);
        ImGui::SetTooltip("%s", ClassNamesVOC[selectedClassID]);
    }

    std::vector<cv::Vec4b> preprocessedColorsVOC(ColorsVOC.size());
    for (uint32_t colorIndex = 0; colorIndex < ColorsVOC.size(); ++colorIndex)
    {
        float alpha = (selectedClassID == colorIndex) ? 0.5f : 0.4f;
        preprocessedColorsVOC[colorIndex] = abgr2bgra(changeAlpha(ColorsVOC[colorIndex], alpha));
    }

    cv::Mat coloredMask(annotation.Mask.size(), CV_8UC4);
    for (int row = 0; row < coloredMask.rows; ++row)
    {
        for (int col = 0; col < coloredMask.cols; ++col)
        {
            auto classID = annotation.Mask.at<uint8_t>(row, col);
            coloredMask.at<cv::Vec4b>(row, col) = preprocessedColorsVOC[classID];
        }
    }

    if (!mSegmentationMask || (mSegmentationMask->width() != (uint32_t)annotation.Mask.cols) || (mSegmentationMask->height() != (uint32_t)annotation.Mask.rows))
        mSegmentationMask = std::make_shared<Texture>(coloredMask);
    else
        mSegmentationMask->setData(coloredMask);

    ImDrawList *drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(reinterpret_cast<ImTextureID>(mSegmentationMask->rendererID()), mInitPos, {mInitPos.x + mViewerSize.x, mInitPos.y + mViewerSize.y});
}

void AnnotationRenderer::operator()(const KeypointsAnnotation&)
{
}

ImU32 AnnotationRenderer::changeAlpha(ImU32 color, float desiredAlpha)
{
    const ImU32 colorMask = 0xffffff;

    float alphaF = desiredAlpha * 255.f;                // from 0.f to 255.f
    ImU32 alpha = (ImU32)(alphaF) << 24;                // alpha is left-most byte
    return (alpha & ~colorMask) | (color & colorMask);  // set alpha and do not change color
}

cv::Vec4b AnnotationRenderer::abgr2bgra(ImU32 color)
{
    static const uint32_t aMask = 0xff000000;
    static const uint32_t bMask = 0x00ff0000;
    static const uint32_t gMask = 0x0000ff00;
    static const uint32_t rMask = 0x000000ff;

    return {uint8_t((color & bMask) >> 16), uint8_t((color & gMask) >> 8), uint8_t(color & rMask), uint8_t((color & aMask) >> 24)};
}

}
