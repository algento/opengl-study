/**
 * @file texture.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once

#include "gl-core/renderer/texture_specification.h"
namespace glcore {
class Texture2D {
 public:
    Texture2D(uint32_t width, uint32_t height);
    ~Texture2D();

    Texture2D(const Texture2D&)            = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&&)                 = default;
    Texture2D& operator=(Texture2D&&)      = default;

    [[nodiscard]] uint32_t width() const { return width_; }
    [[nodiscard]] uint32_t height() const { return height_; }
    [[nodiscard]] uint32_t renderer_id() const { return renderer_id_; }
    [[nodiscard]] const TextureSpec& spec() const { return spec_; }
    [[nodiscard]] TextureSpec& spec() { return spec_; }

    void SetData(void* data, uint32_t size);
    void Bind();
    void Unbind();

    bool operator==(const Texture2D& other) const {
        return renderer_id_ == other.renderer_id_;
    }

 private:
    std::string path_;
    TextureSpec spec_;
    uint32_t width_{0}, height_{0};
    uint32_t renderer_id_{0};
    uint32_t internal_format_{0}, data_format_{0};
};

}  // namespace glcore