/**
 * @file texture.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */

#include <gl-core/renderer/texture.h>
#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

#include <cstdint>

#include "gl-core/base/exception.h"

namespace glcore {

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

Texture2D::Texture2D(TextureImage&& image, Option option)
    : image_(std::move(image)), option_(option) {
    // Generate texture
#if defined(_WIN32)
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
    glTextureStorage2D(renderer_id_, 1, GetGLInternalFormat(), spec_.width,
                       spec_.height);
#elif defined(__APPLE__)
    glGenTextures(1, &renderer_id_);
#endif
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, renderer_id_);

    // Set texture options
    SetTextureWrap();
    SetTextureFilter();

    SetData();

    Unbind();
}

Texture2D::~Texture2D() { glDeleteTextures(1, &renderer_id_); }

void Texture2D::SetData() const {
#if defined(_WIN32)
    glTextureSubImage2D(renderer_id_, 0, 0, 0, (GLsizei)GetWidth(),
                        (GLsizei)GetHeight(), GetDataFormat(), GetGLDataType(),
                        data);
#elif defined(__APPLE__)
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GetGLInternalFormat(),
                 (GLsizei)GetWidth(), (GLsizei)GetHeight(), 0, GetDataFormat(),
                 GetGLDataType(), image_.buffer().data());
#endif
    glGenerateMipmap(GL_TEXTURE_2D);
    Unbind();
}

void Texture2D::Bind(uint32_t slot) const {
#if defined(_WIN32)
    glBindTextureUnit(slot, renderer_id_);
#elif defined(__APPLE__)
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);
#endif
}

void Texture2D::SetTextureFilter() const {
    int32_t mag_filter{0};
    int32_t min_filter{0};
    switch (option_.filter) {
        case Texture::Filter::kLinear:
            mag_filter = GL_LINEAR;
            min_filter = GL_LINEAR;
            break;
        case Texture::Filter::kNearest:
            mag_filter = GL_NEAREST;
            min_filter = GL_NEAREST;
            break;
        case Texture::Filter::kNearest_MIP_Linear:
            mag_filter = GL_NEAREST;
            min_filter = GL_NEAREST_MIPMAP_LINEAR;
            break;
        case Texture::Filter::kNearest_MIP_Nearest:
            mag_filter = GL_NEAREST;
            min_filter = GL_NEAREST_MIPMAP_NEAREST;
            break;
        case Texture::Filter::kLinear_MIP_Nearest:
            mag_filter = GL_LINEAR;
            min_filter = GL_LINEAR_MIPMAP_NEAREST;
            break;
        case Texture::Filter::kLinear_MIP_Linear:
            mag_filter = GL_LINEAR;
            min_filter = GL_LINEAR_MIPMAP_LINEAR;
            break;
        default:
            mag_filter = GL_LINEAR;
            min_filter = GL_LINEAR;
            break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

void Texture2D::SetTextureWrap() const {
    int32_t wrap_option = 0;
    switch (option_.wrap) {
        case Texture::Wrap::kRepeat:
            wrap_option = GL_REPEAT;
            break;
        case Texture::Wrap::kMirroredRepeat:
            wrap_option = GL_MIRRORED_REPEAT;
            break;
        case Texture::Wrap::kClampToEdge:
            wrap_option = GL_CLAMP_TO_EDGE;
            break;
        case Texture::Wrap::kClampToBorder:
            wrap_option = GL_CLAMP_TO_BORDER;
            break;
        default:
            wrap_option = GL_REPEAT;
            break;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_option);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_option);
}

uint32_t Texture2D::GetGLInternalFormat() const {
    switch (image_.format()) {
        case TextureImage::Format::kR32I:
            return GL_R32I;
        case TextureImage::Format::kR32UI:
            return GL_R32UI;
        case TextureImage::Format::kRGB8:
            return GL_RGB8;
        case TextureImage::Format::kRGBA8:
            return GL_RGBA8;
        case TextureImage::Format::kRGBA32F:
            return GL_RGBA32F;
        case TextureImage::Format::kDEPTH24_STENCIL8:
            return GL_DEPTH24_STENCIL8;
        default:
            GLCORE_ASSERT_(image_.format() ==  //NOLINT
                           TextureImage::Format::kNone);
            return 0;
    }
}

uint32_t Texture2D::GetDataFormat() const {
    switch (image_.format()) {
        case TextureImage::Format::kRGB8:
            return GL_RGB;
        case TextureImage::Format::kRGBA8:
        case TextureImage::Format::kRGBA32F:
            return GL_RGBA;
        case TextureImage::Format::kR32I:
        case TextureImage::Format::kR32UI:
            return GL_RED_INTEGER;
        case TextureImage::Format::kDEPTH24_STENCIL8:
            return GL_DEPTH_STENCIL;
        default:
            GLCORE_ASSERT_(image_.format() ==  //NOLINT
                           TextureImage::Format::kNone);
            return 0;
    }
}

uint32_t Texture2D::GetGLDataType() const {
    switch (image_.format()) {
        case TextureImage::Format::kRGB8:
        case TextureImage::Format::kRGBA8:
            return GL_UNSIGNED_BYTE;
        case TextureImage::Format::kRGBA32F:
            return GL_FLOAT;
        case TextureImage::Format::kR32I:
            return GL_INT;
        case TextureImage::Format::kR32UI:
            return GL_UNSIGNED_INT;
        case TextureImage::Format::kDEPTH24_STENCIL8:
            return GL_UNSIGNED_INT_24_8;
        default:
            GLCORE_ASSERT_(image_.format() ==  //NOLINT
                           TextureImage::Format::kNone);
            return 0;
    }
}

uint32_t Texture2D::GetGLBytePerPixel() const {
    switch (image_.format()) {
        case TextureImage::Format::kRGB8:
            return 3;
        case TextureImage::Format::kR32I:
        case TextureImage::Format::kR32UI:
        case TextureImage::Format::kDEPTH24_STENCIL8:
        case TextureImage::Format::kRGBA8:
            return 4;
        case TextureImage::Format::kRGBA32F:
            return 4 * 4;
        default:
            GLCORE_ASSERT_(image_.format() ==  //NOLINT
                           TextureImage::Format::kNone);
            return 0;
    }
}
}  // namespace glcore