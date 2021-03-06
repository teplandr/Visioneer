#pragma once

#include <imgui.h>

#include "Visioneer/Core/Texture.h"
#include "Visioneer/Core/Annotation.h"

namespace Visioneer
{

class AnnotationRenderer
{
public:
    void setInitPos(const ImVec2& initPos, const ImVec2& size) { mInitPos = initPos; mViewerSize = size; }
    void setMousePos(const ImVec2& mousePos) { mMousePos = mousePos; }

    void operator()(const EmptyAnnotation& annotation);
    void operator()(const BBoxesAnnotation& annotation);
    void operator()(const KeypointsAnnotation& annotation);
    void operator()(const SemanticSegmentAnnotation& annotation);

private:
    ImU32 changeAlpha(ImU32 color, float desiredAlpha);
    cv::Vec4b abgr2bgra(ImU32 color);

private:
    ImVec2 mInitPos;
    ImVec2 mViewerSize;
    ImVec2 mMousePos;

    std::shared_ptr<Texture> mSegmentationMask = nullptr;
};

}
