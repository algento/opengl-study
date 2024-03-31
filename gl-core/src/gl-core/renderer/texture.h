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

#include <sys/stat.h>

#include <cstdint>

#include "gl-core/renderer/raw_buffer.h"
#include "gl-core/renderer/texture_image.h"
namespace glcore {
//TODO: Asset Class for varrious types of assets?

class Texture : private MovableNonCopyable {
 public:
    enum class TextureType { kNone = 0, kTexture2D, kTexture3D };
    enum class Filter : int32_t {
        kLinear = 1,
        kNearest,
        kNearest_MIP_Linear,
        kNearest_MIP_Nearest,
        kLinear_MIP_Nearest,
        kLinear_MIP_Linear
    };
    enum class Wrap : int32_t {
        kClampToEdge = 1,
        kClampToBorder,
        kRepeat,
        kMirroredRepeat,
    };

    struct Option {
        Option()         = default;
        TextureType type = TextureType::kTexture2D;
        Filter filter    = Filter::kLinear;
        Wrap wrap        = Wrap::kClampToEdge;
    };

    virtual void Bind(uint32_t slot = 0) const = 0;
    static void Unbind();
    [[nodiscard]] virtual RawBuffer& GetBuffer()             = 0;
    [[nodiscard]] virtual const RawBuffer& GetBuffer() const = 0;

    [[nodiscard]] virtual TextureImage::Format GetImageFormat() const = 0;
    [[nodiscard]] virtual uint32_t GetWidth() const                   = 0;
    [[nodiscard]] virtual uint32_t GetHeight() const                  = 0;
    [[nodiscard]] virtual float GetAspectRatio() const                = 0;
};

class Texture2D : public Texture {
 public:
    explicit Texture2D(TextureImage&& image, Option option = Option());
    ~Texture2D() override;

    Texture2D(const Texture2D&)            = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&&)                 = default;
    Texture2D& operator=(Texture2D&&)      = default;

    void Bind(uint32_t slot = 0) const override;

    [[nodiscard]] TextureImage::Format GetImageFormat() const override {
        return image_.format();
    }
    [[nodiscard]] uint32_t GetWidth() const override { return image_.width(); }
    [[nodiscard]] uint32_t GetHeight() const override {
        return image_.height();
    }
    [[nodiscard]] float GetAspectRatio() const override {
        return static_cast<float>(image_.width()) /
               static_cast<float>(image_.height());
    }
    [[nodiscard]] const RawBuffer& GetBuffer() const override {
        return image_.buffer();
    }
    [[nodiscard]] RawBuffer& GetBuffer() override { return image_.buffer(); }

    [[nodiscard]] uint32_t renderer_id() const { return renderer_id_; }

    bool operator==(const Texture2D& other) const {
        return renderer_id_ == other.renderer_id_;
    }

    bool operator!=(const Texture2D& other) const {
        return renderer_id_ != other.renderer_id_;
    }

 private:
    void SetTextureFilter() const;
    void SetTextureWrap() const;
    [[nodiscard]] uint32_t GetGLInternalFormat() const;
    [[nodiscard]] uint32_t GetGLDataType() const;
    [[nodiscard]] uint32_t GetGLBytePerPixel() const;
    [[nodiscard]] uint32_t GetDataFormat() const;
    void SetData() const;

    TextureImage image_;
    Option option_;
    uint32_t width_{0}, height_{0};
    uint32_t renderer_id_{0};
    uint32_t internal_format_{0}, data_format_{0};
};

}  // namespace glcore