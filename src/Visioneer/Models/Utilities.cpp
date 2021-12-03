#include <stdint.h>

#include "Visioneer/Core/Base.h"
#include "Visioneer/Models/Utilities.h"

namespace Visioneer
{

inline float Utilities::computeIoU(const Rect& lhs, const Rect& rhs)
{
    float left   = std::max(lhs.X1, rhs.X1);
    float top    = std::max(lhs.Y1, rhs.Y1);
    float right  = std::min(lhs.X2, rhs.X2);
    float bottom = std::min(lhs.Y2, rhs.Y2);

    float intersectionArea = std::max(0.f, right - left) * std::max(0.f, bottom - top);
    return intersectionArea / (lhs.area() + rhs.area() - intersectionArea);
}

void Utilities::nonMaximumSupperession(std::vector<BBoxesAnnotation::Item> &items, float suppressionThreshold)
{
    std::sort(items.begin(), items.end(), [](const auto& lhs, const auto& rhs)
    {
        return lhs.Score > rhs.Score;
    });

    std::vector<uint32_t> indices;
    indices.reserve(items.size() / 2);

    for (uint32_t i = 0; i < items.size(); ++i)
    {
        const auto& a = items.at(i);

        bool keep = true;
        for (uint32_t j = 0; j < indices.size(); ++j)
        {
            const auto& b = items.at(indices.at(j));
            if (computeIoU(a.BBox, b.BBox) > suppressionThreshold)
                keep = false;
        }

        if (keep)
            indices.push_back(i);
    }

    std::vector<BBoxesAnnotation::Item> result;
    result.reserve(indices.size());
    for (uint32_t index : indices)
        result.push_back(items.at(index));

    VSR_TRACE("Before NMS: {}, After NMS: {}", items.size(), result.size());

    items = result;
}

}
