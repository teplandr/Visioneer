#include "AnnotationRenderer.h"

namespace Visioneer
{

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
        drawList->AddRect(tl, br, 0xFF0000FF, 0.f, 0, 2.f);

        if (mMousePos.x >= tl.x && mMousePos.x <= br.x && mMousePos.y >= tl.y && mMousePos.y <= br.y)
            ImGui::SetTooltip("Score: %f\nClass: %d", score, classID);
    }
    // drawList->AddCircle(mMousePos, 7.f, 0xFF00FFFF, 0, 3.f);

}

void AnnotationRenderer::operator()(const KeypointsAnnotation&)
{
}

void AnnotationRenderer::operator()(const SemSegmentAnnotation&)
{
}

}
