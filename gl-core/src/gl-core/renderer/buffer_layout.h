/**
 * @file buffer_layout.h
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

class GlBufferElement {
 public:
    enum class DataType : uint32_t {
        kNone = 0,
        kFloat,
        kFloat2,
        kFloat3,
        kFloat4,
        kInt,
        kInt2,
        kInt3,
        kInt4,
        kUint,
        kUint2,
        kUint3,
        kUint4,
        kMat3,
        kMat4,
        kBool
    };

    enum class AttribType : uint32_t { kNone = 0, kFloat, kInt, kMat };
    enum class GLBaseType : uint32_t { kNone = 0, kInt, kUint, kFloat, kBool };

    GlBufferElement() = default;
    GlBufferElement(std::string name, DataType type, bool normalized = false)
        : name_(std::move(name)),
          type_(type),
          size_(GetDataTypeSize()),
          is_normalized_(normalized) {}

    [[nodiscard]] int32_t GetComponentCount() const {
        // #lizard forgives the complexity
        switch (type_) {
            case DataType::kFloat:
            case DataType::kInt:
            case DataType::kUint:
            case DataType::kBool:
                return 1;
            case DataType::kFloat2:
            case DataType::kInt2:
            case DataType::kUint2:
                return 2;
            case DataType::kFloat3:
            case DataType::kInt3:
            case DataType::kUint3:
            case DataType::kMat3:
                return 3;
            case DataType::kFloat4:
            case DataType::kInt4:
            case DataType::kUint4:
            case DataType::kMat4:
                return 4;
            default:
                break;
        }
        return 0;
    }

    [[nodiscard]] uint32_t GetDataTypeSize() const {
        // #lizard forgives the complexity
        switch (type_) {
            case DataType::kFloat:
            case DataType::kInt:
            case DataType::kUint:
                return 4;
            case DataType::kFloat2:
            case DataType::kInt2:
            case DataType::kUint2:
                return 4 * 2;
            case DataType::kFloat3:
            case DataType::kInt3:
            case DataType::kUint3:
                return 4 * 3;
            case DataType::kFloat4:
            case DataType::kInt4:
            case DataType::kUint4:
                return 4 * 4;
            case DataType::kMat3:
                return 4 * 3 * 3;
            case DataType::kMat4:
                return 4 * 4 * 4;
            case DataType::kBool:
                return 1;
            default:
                break;
        }
        return 0;
    }

    [[nodiscard]] AttribType GetAttribType() const {
        // #lizard forgives the complexity
        switch (type_) {
            case DataType::kFloat:
            case DataType::kFloat2:
            case DataType::kFloat3:
            case DataType::kFloat4:
                return AttribType::kFloat;
            case DataType::kInt:
            case DataType::kInt2:
            case DataType::kInt3:
            case DataType::kInt4:
            case DataType::kUint:
            case DataType::kUint2:
            case DataType::kUint3:
            case DataType::kUint4:
            case DataType::kBool:
                return AttribType::kInt;
            case DataType::kMat3:
            case DataType::kMat4:
                return AttribType::kMat;
            default:
                return AttribType::kNone;
        }
    }

    [[nodiscard]] GLBaseType GetGLBaseType() const {
        // #lizard forgives the complexity
        switch (type_) {
            case DataType::kInt:
            case DataType::kInt2:
            case DataType::kInt3:
            case DataType::kInt4:
                return GLBaseType::kInt;  // GL_INT
            case DataType::kFloat:
            case DataType::kFloat2:
            case DataType::kFloat3:
            case DataType::kFloat4:
            case DataType::kMat3:
            case DataType::kMat4:
                return GLBaseType::kFloat;  // GL_FLOAT
            case DataType::kUint:
            case DataType::kUint2:
            case DataType::kUint3:
            case DataType::kUint4:
                return GLBaseType::kUint;  // GL_UNSIGNED_INT
            case DataType::kBool:
                return GLBaseType::kBool;  // GL_BOOL
            default:
                return GLBaseType::kNone;
        }
    }

    [[nodiscard]] const std::string& name() const { return name_; }
    [[nodiscard]] DataType type() const { return type_; }
    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] size_t offset() const { return offset_; }
    [[nodiscard]] bool is_normalized() const { return is_normalized_; }

    void SetOffset(size_t offset) { offset_ = offset; }

 private:
    std::string name_{};
    DataType type_{DataType::kNone};
    size_t size_{0};
    size_t offset_{0};
    bool is_normalized_{false};
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
            element.SetOffset(offset);
            offset += element.size();
        }
    }

    void UpdateStride() {
        stride_ = 0;
        if (elements_.empty()) {
            return;
        }
        stride_ = static_cast<int32_t>(elements_.back().size());
    }

    std::vector<GlBufferElement> elements_{};
    int32_t stride_{0};
};

}  // namespace glcore
