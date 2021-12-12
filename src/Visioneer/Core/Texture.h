#pragma once

#include <string>

#include <opencv2/core.hpp>

namespace Visioneer
{

class Texture
{
public:
    Texture(uint32_t width, uint32_t height);
    Texture(const cv::Mat& image);
    ~Texture();

    void setData(const cv::Mat& image);

    uint32_t width() const { return mWidth;  }
    uint32_t height() const { return mHeight; }
    uint32_t rendererID() const { return mRendererID; }

private:
    uint32_t mWidth, mHeight;
    uint32_t mDataFormat, mInternalFormat;
    uint32_t mRendererID;
};

}
