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

    Rect() = default;
    Rect(float x1, float y1, float x2, float y2)
        : X1(x1), Y1(y1), X2(x2), Y2(y2)
    {}

    float area() const { return (X2 - X1) * (Y2 - Y1); }
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

    struct Item
    {
        Rect  BBox;
        float Score;
        int   ClassID;

        Item() = default;
        Item(const Rect& bbox, float score, int classID)
            : BBox(bbox), Score(score), ClassID(classID)
        {}
    };

    DatasetType       Type;
    std::vector<Item> Items;
};

struct SemanticSegmentAnnotation
{
}; 

struct KeypointsAnnotation
{
};

using VariantAnnotation = std::variant<EmptyAnnotation,
                                       BBoxesAnnotation,
                                       SemanticSegmentAnnotation,
                                       KeypointsAnnotation>;

}
