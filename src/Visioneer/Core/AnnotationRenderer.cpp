#include "AnnotationRenderer.h"

#include <array>

namespace Visioneer
{

static std::array<const char*, 80> ClassNamesCOCO
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

static std::array<uint32_t, 80> ColorsCOCO
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

void AnnotationRenderer::operator()(const EmptyAnnotation&)
{
}

void AnnotationRenderer::operator()(const BBoxesAnnotation& annotation)
{
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    for (size_t i = 0; i < annotation.BBoxes.size(); ++i)
    {
        const Rect& currentBBox = annotation.BBoxes.at(i);
        float score = annotation.Scores.at(i);
        int classID = annotation.Classes.at(i);

        ImVec2 tl(mInitPos.x + currentBBox.X1 * mSize.x, mInitPos.y + currentBBox.Y1 * mSize.y);
        ImVec2 br(mInitPos.x + currentBBox.X2 * mSize.x, mInitPos.y + currentBBox.Y2 * mSize.y);
        drawList->AddRect(tl, br, ColorsCOCO[classID], 1.f, 0, 3.f);

        if (mMousePos.x >= tl.x && mMousePos.x <= br.x && mMousePos.y >= tl.y && mMousePos.y <= br.y)
            ImGui::SetTooltip("%s (%.3f)", ClassNamesCOCO[classID], score);
    }
}

void AnnotationRenderer::operator()(const KeypointsAnnotation&)
{
}

void AnnotationRenderer::operator()(const SemanticSegmentAnnotation&)
{
}

}
