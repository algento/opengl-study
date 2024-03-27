/**
 * @file vertex_buffer.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once

#include <memory>

#include "gl-core/renderer/buffer_layout.h"

namespace glcore {
class VertexBuffer {
 public:
    explicit VertexBuffer(uint32_t size);
    VertexBuffer(void* vertices_data, uint32_t size);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&)                 = default;
    VertexBuffer& operator=(VertexBuffer&&)      = default;

    void Bind() const;
    static void Unbind();

    void SetData(const void* vertices_data, uint32_t size) const;
    void SetLayout(const GlBufferLayout& layout) { layout_ = layout; }

    [[nodiscard]] const GlBufferLayout& GetLayout() const { return layout_; }
    [[nodiscard]] uint32_t GetId() const { return id_; }

    static std::shared_ptr<VertexBuffer> Create(uint32_t size);
    static std::shared_ptr<VertexBuffer> Create(void* vertices_data,
                                                uint32_t size);

 private:
    uint32_t id_{0};
    GlBufferLayout layout_;
};
}  // namespace glcore