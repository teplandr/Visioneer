#pragma once

#include <vector>
#include <variant>

namespace Visioneer
{

struct Rect
{
    float X1;
    float Y1;
    float X2;
    float Y2;

    Rect(float x1, float y1, float x2, float y2)
        : X1(x1), Y1(y1), X2(x2), Y2(y2)
    {}
};

struct EmptyAnnotation
{
};

struct BBoxesAnnotation
{
    enum class DatasetType
    {
        OneClass,
        COCO
    };

    DatasetType        Type;
    std::vector<Rect>  BBoxes;
    std::vector<float> Scores;
    std::vector<int>   Classes;
};

struct KeypointsAnnotation
{
};

struct SemanticSegmentAnnotation
{
};

using VariantAnnotation = std::variant<EmptyAnnotation,
                                       BBoxesAnnotation,
                                       KeypointsAnnotation,
                                       SemanticSegmentAnnotation>;

}
