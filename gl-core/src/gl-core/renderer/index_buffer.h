/**
 * @file index_buffer.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once
#include "gl-core/renderer/buffer_layout.h"

namespace glcore {
class IndexBuffer {
 public:
    explicit IndexBuffer(uint32_t* indices, uint32_t count);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&)            = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&&)                 = default;
    IndexBuffer& operator=(IndexBuffer&&)      = default;

    void Bind() const;
    static void Unbind();

    [[nodiscard]] uint32_t GetCount() const { return count_; }

    static std::shared_ptr<IndexBuffer> Create(uint32_t* indices,
                                               uint32_t count);

 private:
    uint32_t id_{0};
    uint32_t count_{0};
};
}  // namespace glcore