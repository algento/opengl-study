/**
 * @file mesh.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once

#include "gl-core/renderer/buffer_layout.h"
#include "gl-core/renderer/index_buffer.h"
#include "gl-core/renderer/vertex_array.h"
#include "gl-core/renderer/vertex_buffer.h"

namespace glcore {
// TODO: Seperate Mesh and MeshRenderer?
class Mesh {
 public:
    void Create(float* vertex_buffer_data, uint32_t vertex_buffer_size,
                const GlBufferLayout& layout, uint32_t* index_buffer_data,
                uint32_t index_count);
    void Render() const;
    void Release();

    std::shared_ptr<VertexArray> vertex_array_{nullptr};

 private:
    std::shared_ptr<VertexBuffer> vertex_buffer_{nullptr};
    std::shared_ptr<IndexBuffer> index_buffer_{nullptr};
    uint32_t vertex_count_{0};
    uint32_t index_count_{0};
};
}  // namespace glcore