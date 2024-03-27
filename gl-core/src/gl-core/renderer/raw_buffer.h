/**
 * @file buffer.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-24
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once
#include "gl-core/base/exception.h"

namespace glcore {
using Byte = uint8_t;

/// Simple Buffer that holds raw image data
/// @note This Buffer class considers only allocating and releasing memory.
class RawBuffer {
 public:
    RawBuffer() = default;
    RawBuffer(void* data, uint32_t size) : data_(data), size_(size) {}

    ~RawBuffer() { Release(); };

    RawBuffer(const RawBuffer&)            = delete;
    RawBuffer& operator=(const RawBuffer&) = delete;
    RawBuffer(RawBuffer&&)                 = default;
    RawBuffer& operator=(RawBuffer&&)      = default;

    static RawBuffer Copy(const void* data, uint32_t size) {
        RawBuffer buffer;
        buffer.Allocate(size);
        memcpy(buffer.data_, data, size);
        return buffer;
        ;
    }

    void Allocate(uint32_t size) {
        if (size == 0) {
            return;
        }

        size_ = size;
        delete[] (Byte*)data_;
        data_ = new Byte[size];
        std::memset(data_, 0, size);
    }

    void Release() {
        if (data_ != nullptr) {
            delete[] (Byte*)data_;
            data_ = nullptr;
        }
        size_ = 0;
    }

    template <typename T>
    T* Read(uint32_t offset = 0) {
        GLCORE_ASSERT_(offset + sizeof(T) <= size_);
        return (T*)((Byte*)data_ + offset);
    }

    void Write(const void* data, uint32_t size, uint32_t offset = 0) {
        GLCORE_ASSERT_(offset + size < size_);  //NOLINT
        memcpy((Byte*)data_ + offset, data, size);
    }

    explicit operator bool() const { return data_ != nullptr; }

    Byte& operator[](int index) { return ((Byte*)data_)[index]; }

    Byte operator[](int index) const { return ((Byte*)data_)[index]; }

    [[nodiscard]] void* data() const { return data_; }
    [[nodiscard]] uint32_t size() const { return size_; }

 private:
    void* data_{};
    uint32_t size_{0};
};
}  // namespace glcore