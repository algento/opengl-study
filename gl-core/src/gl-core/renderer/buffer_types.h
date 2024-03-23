/**
 * @file buffer_types.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace glcore {
enum class GlDataType : uint32_t {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Uint,
    Uint2,
    Uint3,
    Uint4,
    Mat3,
    Mat4,
    Bool
};

inline uint32_t GetDataTypeSize(GlDataType type) {
    // #lizard forgives the complexity
    switch (type) {
        case GlDataType::Float:
        case GlDataType::Int:
        case GlDataType::Uint:
            return 4;
        case GlDataType::Float2:
        case GlDataType::Int2:
        case GlDataType::Uint2:
            return 4 * 2;
        case GlDataType::Float3:
        case GlDataType::Int3:
        case GlDataType::Uint3:
            return 4 * 3;
        case GlDataType::Float4:
        case GlDataType::Int4:
        case GlDataType::Uint4:
            return 4 * 4;
        case GlDataType::Mat3:
            return 4 * 3 * 3;
        case GlDataType::Mat4:
            return 4 * 4 * 4;
        case GlDataType::Bool:
            return 1;
        default:
            break;
    }
    return 0;
}

struct GlBufferElement {
    std::string name{};
    GlDataType type{GlDataType::None};
    size_t size{0};
    size_t offset{0};
    bool normalized{false};

    GlBufferElement() = default;
    GlBufferElement(std::string name, GlDataType type, bool normalized = false)
        : name(std::move(name)),
          type(type),
          size(GetDataTypeSize(type)),
          normalized(normalized) {}
    [[nodiscard]] int32_t GetComponentCount() const {
        // #lizard forgives the complexity
        switch (type) {
            case GlDataType::Float:
            case GlDataType::Int:
            case GlDataType::Uint:
            case GlDataType::Bool:
                return 1;
            case GlDataType::Float2:
            case GlDataType::Int2:
            case GlDataType::Uint2:
                return 2;
            case GlDataType::Float3:
            case GlDataType::Int3:
            case GlDataType::Uint3:
            case GlDataType::Mat3:
                return 3;
            case GlDataType::Float4:
            case GlDataType::Int4:
            case GlDataType::Uint4:
            case GlDataType::Mat4:
                return 4;
            default:
                break;
        }
        return 0;
    }
};

class GlBufferLayout {
 public:
    GlBufferLayout() = default;

    // std::initializer_list는 복사를 하지 않으므로 굳이 참조로 전달할 필요없다.
    GlBufferLayout(std::initializer_list<GlBufferElement> elements)
        : elements_(elements) {
        CalculateOffsets();
        UpdateStride();
    }

    [[nodiscard]] int32_t GetStride() const { return stride_; }
    [[nodiscard]] const std::vector<GlBufferElement>& GetElements() const {
        return elements_;
    }

    std::vector<GlBufferElement>::iterator begin() { return elements_.begin(); }
    std::vector<GlBufferElement>::iterator end() { return elements_.end(); }
    [[nodiscard]] std::vector<GlBufferElement>::const_iterator begin() const {
        return elements_.begin();
    }
    [[nodiscard]] std::vector<GlBufferElement>::const_iterator end() const {
        return elements_.end();
    }

 private:
    void CalculateOffsets() {
        size_t offset = 0;
        stride_       = 0;
        for (auto& element : elements_) {
            element.offset = offset;
            offset += element.size;
        }
    }

    void UpdateStride() {
        stride_ = 0;
        if (elements_.empty()) {
            return;
        }
        stride_ = static_cast<int32_t>(elements_.back().size);
    }

    std::vector<GlBufferElement> elements_{};
    int32_t stride_{0};
};

}  // namespace glcore
