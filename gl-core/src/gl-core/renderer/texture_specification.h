/**
 * @file texture_specification.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once

#include <filesystem>

namespace glcore {
enum class TextureImageFormat {
    kNone = 0,
    kR32I,
    kR32UI,
    kRGB8,
    kRGBA8,
    kRGBA32F,
    kDEPTH24_STENCIL8  // Depth-stencil
};

enum class TextureImageType { kNone = 0, kTexture, kAttachment, kStorage };

enum class TextureWrap { kNone = 0, kClamp, kRepeat };  // Wrap

enum class TextureFilter { kNone = 0, kLinear, kNearest };  //Interpoloation

enum class TextureType { kNone = 0, kTexture2D, kTexture3D };

struct TextureSpec {
    TextureImageFormat format = TextureImageFormat::kRGBA8;
    TextureWrap wrap          = TextureWrap::kRepeat;
    TextureFilter filter      = TextureFilter::kLinear;
    bool is_generate_mips     = true;
    bool storage              = false;
    uint32_t width            = 1;
    uint32_t height           = 1;
    uint32_t mips             = 1;
    std::string name;
};

inline uint32_t GetTextureImageFormatSize(TextureImageFormat format) {
    switch (format) {
        case TextureImageFormat::kR32I:
        case TextureImageFormat::kR32UI:
        case TextureImageFormat::kRGBA8:
            return 4;
        case TextureImageFormat::kRGB8:
            return 3;
        case TextureImageFormat::kRGBA32F:
            return 4 * 4;
        case TextureImageFormat::kDEPTH24_STENCIL8:
            return 2 * 4;  // TODO: check this
        default:
            return 0;
    }
}
}  // namespace glcore