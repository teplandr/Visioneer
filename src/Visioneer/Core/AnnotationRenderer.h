#pragma once

#include <imgui.h>

#include "Visioneer/Core/Annotation.h"

namespace Visioneer
{

class AnnotationRenderer
{
public:
    void setInitPos(const ImVec2& initPos, const ImVec2& size) { mInitPos = initPos; mSize = size; }
    void setMousePos(const ImVec2& mousePos) { mMousePos = mousePos; }

    void operator()(const EmptyAnnotation& annotation);
    void operator()(const BBoxesAnnotation& annotation);
    void operator()(const KeypointsAnnotation& annotation);
    void operator()(const SemanticSegmentAnnotation& annotation);

private:
    ImVec2 mInitPos;
    ImVec2 mSize;
    ImVec2 mMousePos;
};

}
