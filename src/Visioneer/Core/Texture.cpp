#include "Texture.h"
#include "Visioneer/Core/Base.h"

#include <glad/glad.h>

Texture::Texture(uint32_t width, uint32_t height)
    : mWidth(width), mHeight(height)
{
    mInternalFormat = GL_RGBA8;
    mDataFormat = GL_BGRA;

    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, mInternalFormat, static_cast<int>(mWidth), static_cast<int>(mHeight));

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(const cv::Mat &image)
{
    VSR_TRACE("Begin");

    mWidth  = static_cast<uint32_t>(image.cols);
    mHeight = static_cast<uint32_t>(image.rows);

    mInternalFormat = 0;
    mDataFormat = 0;

    switch (image.channels())
    {
        case 1:
        {
            mInternalFormat = GL_R8;
            mDataFormat = GL_RED;
            break;
        }
        case 3:
        {
            mInternalFormat = GL_RGB8;
            mDataFormat = GL_BGR;
            break;
        }
        case 4:
        {
            mInternalFormat = GL_RGBA8;
            mDataFormat = GL_BGRA;
            break;
        }
        default:
        {
            VSR_CRITICAL("internalFormat = {0}, dataFormat = {1}.", mInternalFormat, mDataFormat);
            assert(0 && "Format not supported!");
            return;
        }
    }

    VSR_TRACE("Width = {0}, Height = {1}, Channels = {2}, Step = {3}, Type = {4}", mWidth, mHeight, image.channels(), image.step, image.type());

    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, mInternalFormat, image.cols, image.rows);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureSubImage2D(mRendererID, 0, 0, 0, image.cols, image.rows, mDataFormat, GL_UNSIGNED_BYTE, image.data);

    /* Другой способ загрузки текстуры, для использования нужно определять GL_RGBA в glTexImage2D
    glGenTextures(1, &mRendererID);
    glBindTexture(GL_TEXTURE_2D, mRendererID);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
    */

    VSR_TRACE("End");
}

Texture::~Texture()
{
    VSR_TRACE("Begin");

    VSR_TRACE("Width = {}, Height = {}", mWidth, mHeight);

    glDeleteTextures(1, &mRendererID);

    VSR_TRACE("End");
}

void Texture::setData(const cv::Mat &image)
{
    assert(image.cols == static_cast<int>(mWidth) && image.rows == static_cast<int>(mHeight) && "Texture has wrong size");
    glTextureSubImage2D(mRendererID, 0, 0, 0, static_cast<int>(mWidth), static_cast<int>(mHeight), mDataFormat, GL_UNSIGNED_BYTE, image.data);
}
