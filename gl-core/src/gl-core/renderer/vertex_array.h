/**
 * @file vertex_array.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once

#include "gl-core/renderer/index_buffer.h"
#include "gl-core/renderer/vertex_buffer.h"

namespace glcore {
class VertexArray {
 public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&)            = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&&)                 = default;
    VertexArray& operator=(VertexArray&&)      = default;

    void Bind() const;
    static void Unbind();
    void Release();

    void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer);
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer);

    [[nodiscard]] const std::shared_ptr<VertexBuffer>& vertex_buffer() const {
        return vertex_buffer_;
    }
    [[nodiscard]] const std::shared_ptr<IndexBuffer>& index_buffer() const {
        return index_buffer_;
    }

    static std::shared_ptr<VertexArray> Create();

 private:
    uint32_t id_{0};
    std::shared_ptr<VertexBuffer> vertex_buffer_{nullptr};
    std::shared_ptr<IndexBuffer> index_buffer_{nullptr};
};
}  // namespace glcore
