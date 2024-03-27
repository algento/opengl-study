/**
 * @file texture_image.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-24
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once

#include <string>
#include <utility>

#include "gl-core/base/noncopyable.h"
#include "gl-core/renderer/raw_buffer.h"
namespace glcore {

class TextureImage {
 public:
    enum class Format {
        kNone = 0,
        kR32I,
        kR32UI,
        kRGB8,
        kRGBA8,
        kRGBA32F,
        kDEPTH24_STENCIL8  // Depth/stencil
    };

    enum class Usage { kNone = 0, kTexture, kAttachment, kStorage };

    TextureImage(std::string name, uint32_t width, uint32_t height,
                 Format format, Usage usage)
        : width_(width),
          height_(height),
          format_(format),
          usage_(usage),
          name_(std::move(name)) {
        buffer_.Allocate(width * height * BytesPerPixel(format));
    }

    TextureImage(std::string name, std::string file_path)
        : name_(std::move(name)), file_path_(std::move(file_path)) {
        LoadImage(file_path);
    }
    ~TextureImage() = default;
    MOVABLE_NONCOPYABLE(TextureImage);

    void LoadImage(const std::string& file_path);
    void LoadData(void* data, uint32_t size);
    void Release();

    static uint32_t BytesPerPixel(Format format) {
        switch (format) {
            case Format::kRGB8:
                return 3;
            case Format::kR32I:
            case Format::kR32UI:
            case Format::kRGBA8:
            case Format::kDEPTH24_STENCIL8:  //TODO: check this
                return 4;
            case Format::kRGBA32F:
                return 4 * 4;
            default:
                return 0;
        }
    }
    [[nodiscard]] uint32_t width() const { return width_; }
    [[nodiscard]] uint32_t height() const { return height_; }
    [[nodiscard]] Format format() const { return format_; }
    [[nodiscard]] const std::string& file_path() const { return file_path_; }
    [[nodiscard]] const std::string& name() const { return name_; }
    [[nodiscard]] const RawBuffer& buffer() const { return buffer_; }
    [[nodiscard]] RawBuffer& buffer() { return buffer_; }

    [[nodiscard]] float GetAspectRatio() const {
        return (float)width_ / (float)height_;
    }

 private:
    uint32_t width_{1};
    uint32_t height_{1};
    Format format_{Format::kNone};
    Usage usage_{Usage::kNone};

    RawBuffer buffer_{};

    std::string name_{};
    std::string file_path_{};
};
};  // namespace glcore