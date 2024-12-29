/**
 * @file texture_image.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-24
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */
#include "gl-core/renderer/texture_image.h"

#include <stb_image/stb_image.h>

#include "gl-core/base/exception.h"
#include "gl-core/renderer/raw_buffer.h"

namespace glcore {

void TextureImage::LoadImage(const std::string& file_path) {
    int32_t channels{1};

    // Load image using stb_image
    stbi_set_flip_vertically_on_load(0);
    Byte* data = stbi_load(file_path.c_str(), (int*)&width_, (int*)&height_,
                           (int*)&channels, 0);
    std::cout << "Image loaded: " << file_path << " (" << width_ << "x"
              << height_ << ")\n";
    GLCORE_ASSERT_(data != nullptr);  //NOLINT

    // Set format from channels
    if (channels == 4) {
        format_ = Format::kRGBA8;
    } else if (channels == 3) {
        format_ = Format::kRGB8;
    } else {
        GLCORE_ASSERTMSG_(false, "Unsupported image format");  //NOLINT
    }

    buffer_ = RawBuffer::Copy(data, BytesPerPixel(format_) * width_ * height_);
    stbi_image_free(data);
}

void TextureImage::LoadData(void* data, uint32_t size) {
    buffer_ = RawBuffer::Copy(data, size);
}

void TextureImage::Release() { buffer_.Release(); }
}  // namespace glcore