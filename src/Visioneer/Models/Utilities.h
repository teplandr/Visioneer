#pragma once

#include "Visioneer/Core/Annotation.h"

namespace Visioneer
{

class Utilities
{
public:
    static inline float computeIoU(const Rect& lhs, const Rect& rhs);
    static void nonMaximumSupperession(std::vector<BBoxesAnnotation::Item>& items, float suppressionThreshold);
};

}
